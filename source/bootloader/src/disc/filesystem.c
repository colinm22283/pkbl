#include <stddef.h>

#include <disc/filesystem.h>
#include <disc/read.h>
#include <disc/write.h>
#include <disc/ports.h>

#include <string/strcmp.h>
#include <string/strcmpn.h>

#include <memory/memcpy.h>
#include <string/strcpy.h>

#include <console/print.h>
#include <console/newline.h>
#include <console/print_dec.h>
#include <console/print_hex.h>

#include <sys/ports.h>

static inline filesystem_page_address_t advance_root_first_free(filesystem_page_address_t first_free) {
    while (true) {
        first_free++;

        filesystem_node_page_t file_node;

        disc_read28(disc_io_port, DISC_READ28_MASTER, first_free, 1, (void *) &file_node);

        if (!file_node.tag.in_use) return first_free;
    }
}

static inline bool remove_page(filesystem_root_page_t * root_page, filesystem_page_address_t address, filesystem_node_page_t * node_page) {
    node_page->tag.in_use = false;
    if (address < root_page->first_free) root_page->first_free = address;

    if (disc_write28(disc_io_port, DISC_WRITE28_MASTER, address, 1, (uint16_t *) &node_page) != 0) return false;

    return true;
}

directory_t open_filesystem(filesystem_page_address_t root_address) {
    static filesystem_root_page_t root_page;
    uint32_t read_result = disc_read28(disc_io_port, DISC_READ28_MASTER, root_address, 1, (void *) &root_page);

    if (strcmpn(root_page.signature, FILESYSTEM_ROOT_SIGNATURE, 4) != 0) {
        return 0;
    }

    return root_page.root_directory_address;
}

directory_t open_filesystem_ex(filesystem_page_address_t root_address, port_t _io_port, bool slave) {
    filesystem_root_page_t root_page;
    if (disc_read28(_io_port, slave ? DISC_READ28_SLAVE : DISC_READ28_MASTER, root_address, 1, (void *) &root_page) != 0) return 0;

    if (strcmpn(root_page.signature, FILESYSTEM_ROOT_SIGNATURE, 4) != 0) {
        return 0;
    }

    return root_page.root_directory_address;
}

directory_t open_directory(directory_t parent, const char * directory_name) {
    filesystem_directory_node_page_t directory_page;
    if (disc_read28(disc_io_port, DISC_READ28_MASTER, parent, 1, (void *) &directory_page)) {
        return 0;
    }

    filesystem_page_address_t directory_index_address = directory_page.directory_index_address;

    while (true) {
        filesystem_directory_index_page_t directory_index;
        if (disc_read28(disc_io_port, DISC_READ28_MASTER, directory_index_address, 1, (void *) &directory_index)) {
            return 0;
        }

        for (size_t i = 0; i < FILESYSTEM_DIRECTORY_INDEX_CHILDREN_SIZE; i++) {
            if (directory_index.children[i] == 0) {
                return 0;
            }
            else {
                filesystem_directory_node_page_t directory_node;
                if (disc_read28(disc_io_port, DISC_READ28_MASTER, directory_index.children[i], 1, (void *) &directory_node)) {
                    return 0;
                }

                if (strcmp(directory_node.name, directory_name) == 0) {
                    return directory_index.children[i];
                }
            }
        }

        if (directory_index.next_index_address != 0) {
            directory_index_address = directory_index.next_index_address;
        }
        else return 0;
    }
}
file_t open_file(directory_t parent, const char * directory_name) {
    filesystem_directory_node_page_t directory_page;
    if (disc_read28(disc_io_port, DISC_READ28_MASTER, parent, 1, (void *) &directory_page)) {
        return 0;
    }

    filesystem_page_address_t directory_index_address = directory_page.directory_index_address;

    while (true) {
        filesystem_directory_index_page_t directory_index;
        if (disc_read28(disc_io_port, DISC_READ28_MASTER, directory_index_address, 1, (void *) &directory_index)) {
            return 0;
        }

        for (size_t i = 0; i < FILESYSTEM_DIRECTORY_INDEX_CHILDREN_SIZE; i++) {
            if (directory_index.children[i] == 0) {
                return 0;
            }
            else {
                filesystem_file_node_page_t file_node;
                if (disc_read28(disc_io_port, DISC_READ28_MASTER, directory_index.children[i], 1, (void *) &file_node)) {
                    return 0;
                }

                if (strcmp(file_node.name, directory_name) == 0) {
                    return directory_index.children[i];
                }
            }
        }

        if (directory_index.next_index_address != 0) {
            directory_index_address = directory_index.next_index_address;
        }
        else return 0;
    }
}

bool get_directory_name(directory_t file, char * buffer) {
    filesystem_directory_node_page_t directory_node;

    if (disc_read28(disc_io_port, DISC_READ28_MASTER, file, 1, (void *) &directory_node)) {
        return false;
    }
    uint16_t i;
    for (i = 0; i < FILESYSTEM_NAME_MAX_SIZE && directory_node.name[i] != '\0'; i++) buffer[i] = directory_node.name[i];
    buffer[i] = '\0';

    return true;
}
bool get_file_name(file_t file, char * buffer) {
    filesystem_file_node_page_t file_node;

    if (disc_read28(disc_io_port, DISC_READ28_MASTER, file, 1, (void *) &file_node)) {
        return false;
    }
    uint16_t i;
    for (i = 0; i < FILESYSTEM_NAME_MAX_SIZE && file_node.name[i] != '\0'; i++) buffer[i] = file_node.name[i];
    buffer[i] = '\0';

    return true;
}

bool directory_iterator_init(directory_iterator_t * iterator, directory_t directory) {
    if (disc_read28(disc_io_port, DISC_READ28_MASTER, directory, 1, (void *) &iterator->node_page)) {
        return false;
    }
    if (disc_read28(disc_io_port, DISC_READ28_MASTER, iterator->node_page.directory_index_address, 1, (void *) &iterator->index_page)) {
        return false;
    }

    iterator->index_location = 0;

    return true;
}

filesystem_directory_entry_type_t directory_iterator_next(directory_iterator_t * iterator, filesystem_page_address_t * handle) {
    if (iterator->index_location >= FILESYSTEM_DIRECTORY_INDEX_CHILDREN_SIZE) {
        if (disc_read28(disc_io_port, DISC_READ28_MASTER, iterator->index_page.next_index_address, 1, (void *) &iterator->index_page)) {
            return FS_DET_NONE;
        }

        iterator->index_location = 0;

        return directory_iterator_next(iterator, handle);
    }
    else if (iterator->index_page.children[iterator->index_location] == 0) {
        return FS_DET_NONE;
    }
    else {
        *handle = iterator->index_page.children[iterator->index_location];
        iterator->index_location++;

        filesystem_node_page_t page;

        if (disc_read28(disc_io_port, DISC_READ28_MASTER, *handle, 1, (void *) &page)) {
            return FS_DET_NONE;
        }

        switch (page.type) {
            case FILESYSTEM_PAGE_TYPE_FILE: return FS_DET_FILE;
            case FILESYSTEM_PAGE_TYPE_DIRECTORY: return FS_DET_DIRECTORY;
            default: return FS_DET_NONE;
        }
    }
}

bool file_reader_init(file_reader_t * reader, file_t file) {
    filesystem_file_node_page_t file_node;
    if (disc_read28(disc_io_port, DISC_READ28_MASTER, file, 1, (void *) &file_node)) {
        return false;
    }

    if (disc_read28(disc_io_port, DISC_READ28_MASTER, file_node.root_data_address, 1, (void *) &reader->data_page)) {
        return false;
    }

    reader->data_address = file_node.root_data_address;
    reader->data_location = 0;

    return true;
}
uint32_t file_reader_read(file_reader_t * reader, char * buffer, uint32_t bytes) {
    if (reader->data_address == 0) return 0;

    uint32_t read_bytes = 0;

    while (reader->data_address != 0 && bytes > 0) {
        if (reader->data_location + bytes >= reader->data_page.size) {
            memcpy(&buffer[read_bytes], &reader->data_page.data[reader->data_location], reader->data_page.size - reader->data_location);
            read_bytes += reader->data_page.size - reader->data_location;
            bytes -= reader->data_page.size - reader->data_location;
            reader->data_location = 0;

            reader->data_address = reader->data_page.next_data_address;
            if (disc_read28(disc_io_port, DISC_READ28_MASTER, reader->data_address, 1, (void *) &reader->data_page)) {
                return 0;
            }
        }
        else {
            memcpy(&buffer[read_bytes], &reader->data_page.data[reader->data_location], bytes);
            read_bytes += bytes;
            reader->data_location += bytes;
            bytes = 0;
        }
    }

    return read_bytes;
}

directory_t open_directory_path(directory_t root, const char * path) {
    directory_t directory = root;

    uint32_t base_addr = 0;
    uint32_t i;
    for (i = 0; path[i] != '\0'; i++) {
        if (path[i] == '/') {
            char buffer[FILESYSTEM_NAME_MAX_SIZE];
            memcpy(buffer, &path[base_addr], i - base_addr);
            buffer[i - base_addr] = '\0';

            directory = open_directory(directory, buffer);
            if (directory == 0) return 0;

            base_addr = i + 1;
        }
    }

    char buffer[FILESYSTEM_NAME_MAX_SIZE];
    memcpy(buffer, &path[base_addr], i - base_addr + 1);

    directory_t dir = open_directory(directory, buffer);

    return dir;
}

file_t open_file_path(directory_t root, const char * path) {
    directory_t directory = root;

    uint32_t base_addr = 0;
    uint32_t i;
    for (i = 0; path[i] != '\0'; i++) {
        if (path[i] == '/') {
            char buffer[FILESYSTEM_NAME_MAX_SIZE];
            memcpy(buffer, &path[base_addr], i - base_addr);
            buffer[i - base_addr] = '\0';

            directory = open_directory(directory, buffer);
            if (directory == 0) return 0;

            base_addr = i + 1;
        }
    }

    char buffer[FILESYSTEM_NAME_MAX_SIZE];
    memcpy(buffer, &path[base_addr], i - base_addr + 1);

    file_t file = open_file(directory, buffer);

    return file;
}

filesystem_page_address_t open_node_path(directory_t root, const char * path) {
    directory_t directory = root;

    uint32_t base_addr = 0;
    uint32_t i;
    for (i = 0; path[i] != '\0'; i++) {
        if (path[i] == '/') {
            char buffer[FILESYSTEM_NAME_MAX_SIZE];
            memcpy(buffer, &path[base_addr], i - base_addr);
            buffer[i - base_addr] = '\0';

            directory = open_directory(directory, buffer);
            if (directory == 0) return 0;

            base_addr = i + 1;
        }
    }

    char buffer[FILESYSTEM_NAME_MAX_SIZE];
    memcpy(buffer, &path[base_addr], i - base_addr + 1);

    directory_iterator_t itr;
    directory_iterator_init(&itr, directory);

    filesystem_page_address_t node_address;
    filesystem_directory_entry_type_t det;
    while ((det = directory_iterator_next(&itr, &node_address)) != FS_DET_NONE) {
        filesystem_node_page_t node_page;
        if (disc_read28(disc_io_port, DISC_READ28_MASTER, node_address, 1, (void *) &node_page)) return false;

        switch (det) {
            case FS_DET_FILE: {
                filesystem_file_node_page_t * file_node = (filesystem_file_node_page_t *) &node_page;

                if (strcmp(buffer, file_node->name) == 0) {
                    return node_address;
                }
            } break;
            case FS_DET_DIRECTORY: {
                filesystem_directory_node_page_t * directory_node = (filesystem_directory_node_page_t *) &node_page;

                if (strcmp(buffer, directory_node->name) == 0) {
                    return node_address;
                }
            } break;

            default: break;
        }
    }

    return 0;
}

bool stat_file(file_stat_result_t * result, filesystem_page_address_t address) {
    filesystem_node_page_t _node_page;
    if (disc_read28(disc_io_port, DISC_READ28_MASTER, address, 1, (void *) &_node_page)) return false;

    if (_node_page.type == FILESYSTEM_PAGE_TYPE_FILE) {
        filesystem_file_node_page_t * node_page = (filesystem_file_node_page_t *) &_node_page;

        uint64_t size = 0;
        uint32_t sectors = 0;

        filesystem_page_address_t data_address = node_page->root_data_address;
        filesystem_file_data_page_t data_page;

        while (data_address != 0) {
            if (disc_read28(disc_io_port, DISC_READ28_MASTER, data_address, 1, (void *) &data_page)) return false;

            size += data_page.size;
            sectors++;

            data_address = data_page.next_data_address;
        }

        result->type = FILESYSTEM_PAGE_TYPE_FILE;
        result->size = size;
        result->size_on_disc = (uint64_t) sectors * 512 + 512;
    }
    else if (_node_page.type == FILESYSTEM_PAGE_TYPE_DIRECTORY) {
        filesystem_directory_node_page_t * node_page = (filesystem_directory_node_page_t *) &_node_page;

        uint32_t sectors = 0;

        filesystem_page_address_t data_address = node_page->directory_index_address;
        filesystem_directory_index_page_t index_page;

        while (data_address != 0) {
            if (disc_read28(disc_io_port, DISC_READ28_MASTER, data_address, 1, (void *) &index_page)) return false;

            sectors++;

            data_address = index_page.next_index_address;
        }

        result->type = FILESYSTEM_PAGE_TYPE_DIRECTORY;
        result->size = result->size_on_disc = (uint64_t) sectors * 512 + 512;
    }
    else return false;

    return true;
}

bool stat_filesystem(filesystem_page_address_t root_page_address, uint64_t * size_on_disk) {
    uint64_t size = 0;

    filesystem_page_address_t address = root_page_address;
    filesystem_node_page_t page;

    while (disc_read28(disc_io_port, DISC_READ28_MASTER, address, 1, (uint16_t *) &page) == 0) {
        if (page.tag.in_use) size++;

        address++;
    }

    *size_on_disk = size * 512;

    return true;
}

bool create_file(filesystem_page_address_t root_page_address, directory_t parent_directory, const char * name) {
    filesystem_root_page_t root_page;
    disc_read28(disc_io_port, DISC_READ28_MASTER, root_page_address, 1, (uint16_t *) &root_page);

    filesystem_directory_node_page_t directory_node_page;
    disc_read28(disc_io_port, DISC_READ28_MASTER, parent_directory, 1, (uint16_t *) &directory_node_page);

    filesystem_page_address_t directory_index_address = directory_node_page.directory_index_address;
    filesystem_directory_index_page_t directory_index_page;

    while (true) {
        disc_read28(disc_io_port, DISC_READ28_MASTER, directory_index_address, 1, (uint16_t *) &directory_index_page);

        for (uint32_t directory_child_index = 0; directory_child_index < FILESYSTEM_DIRECTORY_INDEX_CHILDREN_SIZE; directory_child_index++) {
            if (directory_index_page.children[directory_child_index] == 0) {
                directory_index_page.children[directory_child_index] = root_page.first_free;

                filesystem_page_address_t node_page_address = root_page.first_free;
                root_page.first_free = advance_root_first_free(root_page.first_free);
                filesystem_page_address_t data_page_address = root_page.first_free;
                root_page.first_free = advance_root_first_free(root_page.first_free);

                filesystem_file_node_page_t file_node_page = {
                    .tag.in_use = 1,
                    .type = FILESYSTEM_PAGE_TYPE_FILE,
                    .parent_directory_address = parent_directory,
                    .root_data_address = data_page_address,
                };
                strcpy(file_node_page.name, name);

                filesystem_file_data_page_t file_data_page = {
                    .tag.in_use = 1,
                    .type = FILESYSTEM_PAGE_TYPE_FILE_DATA,
                    .size = 0,
                    .next_data_address = 0,
                    .prev_data_address = 0,
                    .parent_file_address = node_page_address,
                };

                if (disc_write28(disc_io_port, DISC_WRITE28_MASTER, node_page_address, 1, (uint16_t *) &file_node_page) != 0) {
                    return false;
                }

                if (disc_write28(disc_io_port, DISC_WRITE28_MASTER, data_page_address, 1, (uint16_t *) &file_data_page) != 0) {
                    return false;
                }

                if (directory_child_index == FILESYSTEM_DIRECTORY_INDEX_CHILDREN_SIZE - 1) {
                    filesystem_directory_index_page_t new_directory_index = {
                        .tag.in_use = 1,
                        .type = FILESYSTEM_PAGE_TYPE_DIRECTORY_INDEX,
                        .parent_directory_address = parent_directory,
                        .prev_index_address = directory_index_address,
                        .next_index_address = 0,
                    };
                    directory_index_page.next_index_address = root_page.first_free;
                    disc_write28(disc_io_port, DISC_WRITE28_MASTER, root_page.first_free, 1, (uint16_t *) &new_directory_index);

                    root_page.first_free = advance_root_first_free(root_page.first_free);
                }

                disc_write28(disc_io_port, DISC_WRITE28_MASTER, directory_index_address, 1, (uint16_t *) &directory_index_page);

                disc_write28(disc_io_port, DISC_WRITE28_MASTER, root_page_address, 1, (uint16_t *) &root_page);

                return true;
            }
        }

        if (directory_index_page.next_index_address == 0) return false;

        directory_index_address = directory_index_page.next_index_address;
    }
}

bool append_file(filesystem_page_address_t root_address, file_t file, const char * content, uint32_t size) {
    filesystem_root_page_t root_page;
    disc_read28(disc_io_port, DISC_READ28_MASTER, root_address, 1, (uint16_t *) &root_page);

    filesystem_file_node_page_t file_node_page;
    disc_read28(disc_io_port, DISC_READ28_MASTER, file, 1, (void *) &file_node_page);

    filesystem_page_address_t file_data_address = file_node_page.root_data_address;
    filesystem_file_data_page_t file_data_page;

    while (true) {
        disc_read28(disc_io_port, DISC_READ28_MASTER, file_data_address, 1, (void *) &file_data_page);

        uint16_t available_size = FILESYSTEM_FILE_DATA_PAGE_SIZE - file_data_page.size;

        if (available_size == 0) {
            console_print("Finding next page\n");
            file_data_address = file_data_page.next_data_address;
        }
        else break;
    }

    uint32_t current_pos = 0;

    while (current_pos < size) {
        uint16_t remaining_size = size - current_pos;
        uint16_t available_size = FILESYSTEM_FILE_DATA_PAGE_SIZE - file_data_page.size;

        if (available_size > remaining_size) {
            memcpy(file_data_page.data + file_data_page.size, &content[current_pos], remaining_size);
            file_data_page.size += remaining_size;
            disc_write28(disc_io_port, DISC_WRITE28_MASTER, file_data_address, 1, (uint16_t *) &file_data_page);

            current_pos += remaining_size;
        }
        else {
            memcpy(file_data_page.data + file_data_page.size, &content[current_pos], available_size);
            file_data_page.next_data_address = root_page.first_free;
            file_data_page.size += available_size;
            disc_write28(disc_io_port, DISC_WRITE28_MASTER, file_data_address, 1, (uint16_t *) &file_data_page);

            current_pos += available_size;

            filesystem_file_data_page_t new_file_data_page = {
                .tag.in_use = 1,
                .type = FILESYSTEM_PAGE_TYPE_FILE_DATA,
                .parent_file_address = file,
                .prev_data_address = file_data_address,
                .next_data_address = 0,
                .size = 0,
            };
            disc_write28(disc_io_port, DISC_WRITE28_MASTER, root_page.first_free, 1, (uint16_t *) &new_file_data_page);
            root_page.first_free = advance_root_first_free(root_page.first_free);

            file_data_address = file_data_page.next_data_address;
            disc_read28(disc_io_port, DISC_READ28_MASTER, file_data_address, 1, (void *) &file_data_page);
        }
    }

    disc_write28(disc_io_port, DISC_WRITE28_MASTER, root_address, 1, (uint16_t *) &root_page);

    return true;
}

static inline bool remove_directory_node(directory_t dir, filesystem_page_address_t node) {
    filesystem_directory_node_page_t file_parent_node;
    if (disc_read28(disc_io_port, DISC_READ28_MASTER, dir, 1, (uint16_t *) &file_parent_node) != 0) return false;

    filesystem_directory_index_page_t parent_index_page;
    filesystem_page_address_t parent_index_address = file_parent_node.directory_index_address;

    while (parent_index_address != 0) {
        if (disc_read28(disc_io_port, DISC_READ28_MASTER, parent_index_address, 1, (uint16_t *) &parent_index_page) != 0) return false;

        bool page_modified = false;

        uint32_t i;
        for (i = 0; i < FILESYSTEM_DIRECTORY_INDEX_CHILDREN_SIZE; i++) {
            if (parent_index_page.children[i] == node) {
                parent_index_page.children[i] = 0;
                page_modified = true;
                i++;
                break;
            }
        }
        for (; i < FILESYSTEM_DIRECTORY_INDEX_CHILDREN_SIZE; i++) {
            if (parent_index_page.children[i] == 0) break;
            parent_index_page.children[i - 1] = parent_index_page.children[i];
        }

        if (page_modified) {
            if (disc_write28(disc_io_port, DISC_WRITE28_MASTER, parent_index_address, 1, (uint16_t *) &parent_index_page) != 0) return false;

            break;
        }

        parent_index_address = parent_index_page.next_index_address;
    }

    return true;
}

bool delete_file(filesystem_page_address_t root_address, file_t file) {
    filesystem_root_page_t root_page;
    if (disc_read28(disc_io_port, DISC_READ28_MASTER, root_address, 1, (uint16_t *) &root_page) != 0) return false;

    filesystem_file_node_page_t file_node_page;
    if (disc_read28(disc_io_port, DISC_READ28_MASTER, file, 1, (uint16_t *) &file_node_page) != 0) return false;

    if (!remove_directory_node(file_node_page.parent_directory_address, file)) return false;

    filesystem_file_data_page_t file_data_page;
    filesystem_page_address_t file_data_address = file_node_page.root_data_address;

    while (file_data_address != 0) {
        if (disc_read28(disc_io_port, DISC_READ28_MASTER, file_data_address, 1, (uint16_t *) &file_data_page) != 0) return false;

        file_data_page.tag.in_use = false;
        if (file_data_address < root_page.first_free) root_page.first_free = file_data_address;

        if (disc_write28(disc_io_port, DISC_WRITE28_MASTER, file_data_address, 1, (uint16_t *) &file_data_page) != 0) return false;

        file_data_address = file_data_page.next_data_address;
    }

    file_node_page.tag.in_use = false;
    if (disc_write28(disc_io_port, DISC_WRITE28_MASTER, file, 1, (uint16_t *) &file_node_page) != 0) return false;

    if (disc_write28(disc_io_port, DISC_WRITE28_MASTER, root_address, 1, (uint16_t *) &root_page) != 0) return false;

    return true;
}

bool delete_directory(filesystem_page_address_t root_address, directory_t directory) {
    filesystem_root_page_t root_page;
    if (disc_read28(disc_io_port, DISC_READ28_MASTER, root_address, 1, (uint16_t *) &root_page) != 0) return false;

    filesystem_directory_node_page_t directory_node_page;
    if (disc_read28(disc_io_port, DISC_READ28_MASTER, directory, 1, (uint16_t *) &directory_node_page) != 0) return false;

    if (!remove_directory_node(directory_node_page.parent_directory_address, directory)) return false;

    filesystem_directory_index_page_t directory_index_page;
    filesystem_page_address_t directory_index_address = directory_node_page.directory_index_address;

    while (directory_index_address != 0) {
        if (disc_read28(disc_io_port, DISC_READ28_MASTER, directory_index_address, 1, (uint16_t *) &directory_index_page) != 0) return false;

        for (uint32_t i = 0; i < FILESYSTEM_DIRECTORY_INDEX_CHILDREN_SIZE; i++) {
            if (directory_index_page.children[i] == 0) break;

//            remove_page(&root_page, directory, (filesystem_node_page_t *) &directory_node_page);

            if (disc_write28(disc_io_port, DISC_WRITE28_MASTER, root_address, 1, (uint16_t *) &root_page) != 0) return false;

            delete_node(root_address, directory_index_page.children[i]);
        }

        if (disc_write28(disc_io_port, DISC_WRITE28_MASTER, directory_index_address, 1, (uint16_t *) &directory_index_page) != 0) return false;

        directory_index_address = directory_index_page.next_index_address;
    }

    directory_node_page.tag.in_use = false;
    if (disc_write28(disc_io_port, DISC_WRITE28_MASTER, directory, 1, (uint16_t *) &directory_node_page) != 0) return false;

    if (disc_write28(disc_io_port, DISC_WRITE28_MASTER, root_address, 1, (uint16_t *) &root_page) != 0) return false;

    return true;
}

bool delete_node(filesystem_page_address_t root_address, filesystem_page_address_t node) {
    filesystem_node_page_t node_page;
    if (disc_read28(disc_io_port, DISC_READ28_MASTER, node, 1, (uint16_t *) &node_page) != 0) return false;

    switch (node_page.type) {
        case FILESYSTEM_PAGE_TYPE_FILE: {
            if (!delete_file(root_address, node)) return false;
        } break;
        case FILESYSTEM_PAGE_TYPE_DIRECTORY: {
            if (!delete_directory(root_address, node)) return false;
        } break;
        default: return false;
    }

    return true;
}