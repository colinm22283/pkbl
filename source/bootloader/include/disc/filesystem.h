#pragma once

#include <stdint.h>
#include <stdbool.h>

#include <defs.h>

#include <sys/port.h>

#define FILESYSTEM_ROOT_SIGNATURE ("PKFS")

#define FILESYSTEM_NAME_MAX_SIZE (128)
#define FILESYSTEM_FILE_DATA_PAGE_SIZE (512 - sizeof(filesystem_page_tag_t) - sizeof(filesystem_page_type_t) - 3 * sizeof(filesystem_page_address_t) - sizeof(uint16_t))
#define FILESYSTEM_DIRECTORY_INDEX_CHILDREN_SIZE ((512 - sizeof(filesystem_page_tag_t) - sizeof(filesystem_page_type_t) - 3 * sizeof(filesystem_page_address_t)) / sizeof(filesystem_page_address_t))


enum {
    FILESYSTEM_PAGE_TYPE_FILE,
    FILESYSTEM_PAGE_TYPE_FILE_DATA,
    FILESYSTEM_PAGE_TYPE_DIRECTORY,
    FILESYSTEM_PAGE_TYPE_DIRECTORY_INDEX,
};

typedef uint8_t filesystem_page_padding_t[512];

typedef uint64_t filesystem_page_address_t;
typedef uint8_t filesystem_page_type_t;

typedef struct __PACKED {
    uint8_t in_use    : 1;
    uint8_t _reserved : 7;
} filesystem_page_tag_t;

typedef union __PACKED {
    struct __PACKED {
        filesystem_page_tag_t tag;
        const char signature[4];

        filesystem_page_address_t root_directory_address;
        filesystem_page_address_t first_free;
    };

    filesystem_page_padding_t _padding;
} filesystem_root_page_t;

typedef union __PACKED {
    struct __PACKED {
        filesystem_page_tag_t tag;
        filesystem_page_type_t type;
    };

    filesystem_page_padding_t _padding;
} filesystem_node_page_t;

typedef union __PACKED {
    struct __PACKED {
        filesystem_page_tag_t tag;
        filesystem_page_type_t type;
        char name[FILESYSTEM_NAME_MAX_SIZE];

        filesystem_page_address_t parent_directory_address;
        filesystem_page_address_t root_data_address;
    };

    filesystem_page_padding_t _padding;
} filesystem_file_node_page_t;

typedef union __PACKED {
    struct __PACKED {
        filesystem_page_tag_t tag;
        filesystem_page_type_t type;

        filesystem_page_address_t parent_file_address;
        filesystem_page_address_t prev_data_address;
        filesystem_page_address_t next_data_address;

        uint16_t size;

        char data[FILESYSTEM_FILE_DATA_PAGE_SIZE];
    };

    filesystem_page_padding_t _padding;
} filesystem_file_data_page_t;

typedef union __PACKED {
    struct __PACKED {
        filesystem_page_tag_t tag;
        filesystem_page_type_t type;
        char name[FILESYSTEM_NAME_MAX_SIZE];

        filesystem_page_address_t parent_directory_address;
        filesystem_page_address_t directory_index_address;
    };

    filesystem_page_padding_t _padding;
} filesystem_directory_node_page_t;

typedef union __PACKED {
    struct __PACKED {
        filesystem_page_tag_t tag;
        filesystem_page_type_t type;

        filesystem_page_address_t parent_directory_address;
        filesystem_page_address_t prev_index_address;
        filesystem_page_address_t next_index_address;
        filesystem_page_address_t children[FILESYSTEM_DIRECTORY_INDEX_CHILDREN_SIZE];
    };

    filesystem_page_padding_t _padding;
} filesystem_directory_index_page_t;

typedef filesystem_page_address_t directory_t;
typedef struct {
    filesystem_directory_node_page_t node_page;
    filesystem_directory_index_page_t index_page;
    uint16_t index_location;
} directory_iterator_t;
typedef enum { FS_DET_NONE, FS_DET_FILE, FS_DET_DIRECTORY } filesystem_directory_entry_type_t;

typedef filesystem_page_address_t file_t;
typedef struct {
    filesystem_file_data_page_t data_page;
    filesystem_page_address_t data_address;
    uint16_t data_location;
} file_reader_t;

directory_t open_filesystem(filesystem_page_address_t root_address);
directory_t open_filesystem_ex(filesystem_page_address_t root_address, port_t _io_port, bool slave);
directory_t open_directory(directory_t parent, const char * directory_name);
file_t open_file(directory_t parent, const char * directory_name);
directory_t open_directory_path(directory_t root, const char * path);
file_t open_file_path(directory_t root, const char * path);
filesystem_page_address_t open_node_path(directory_t root, const char * path);

bool get_directory_name(directory_t file, char * buffer);
bool get_file_name(file_t file, char * buffer);

bool directory_iterator_init(directory_iterator_t * iterator, directory_t directory);
filesystem_directory_entry_type_t directory_iterator_next(directory_iterator_t * iterator, filesystem_page_address_t * handle);

bool file_reader_init(file_reader_t * reader, file_t file);
uint32_t file_reader_read(file_reader_t * reader, char * buffer, uint32_t bytes);

typedef struct {
    filesystem_page_type_t type;
    uint64_t size;
    uint64_t size_on_disc;
} file_stat_result_t;

bool stat_file(file_stat_result_t * result, filesystem_page_address_t address);
bool stat_filesystem(filesystem_page_address_t root_page_address, uint64_t * size_on_disk);

bool create_file(filesystem_page_address_t root_page_address, directory_t parent_directory, const char * name);
bool append_file(filesystem_page_address_t root_address, file_t file, const char * content, uint32_t size);

bool delete_file(filesystem_page_address_t root_address, file_t file);
bool delete_directory(filesystem_page_address_t root_address, directory_t directory);
bool delete_node(filesystem_page_address_t root_address, filesystem_page_address_t node);