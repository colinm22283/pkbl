#include <stdbool.h>

#include <commands/ed.h>

#include <console/print.h>
#include <console/print_dec.h>
#include <console/clear.h>
#include <console/frame_buffer.h>
#include <console/newline.h>
#include <console/update.h>

#include <shell/input.h>

#include <disc/filesystem.h>

#include <boot/kernel.h>

#include <memory/memcpy.h>

#include <string/strlen.h>

#include <keyboard/getch.h>
#include <keyboard/keycode.h>
#include <keyboard/keyboard.h>

#include <heap/alloc.h>
#include <heap/free.h>

file_t file;

uint32_t line_count;
char ** line_buffer;
uint32_t * line_capacities;

uint32_t cur_x, cur_y;
uint32_t frame_pos = 0;

void ed_init_frame(void) {
    console_char_t * frame_buffer = console_frame_buffer();
    uint32_t width = console_width();
    uint32_t height = console_height();

#define FRAME_BUFFER_GET(x, y) (frame_buffer[(y) * width + (x)])

    for (uint32_t y = 0; y < height; y++) {
        if (y >= line_count) {
            for (uint32_t x = 0; x < width; x++) FRAME_BUFFER_GET(x, y).ch = ' ';
        }
        else {
            uint32_t x = 0;
            while (line_buffer[y][x] != '\0' && x < width) {
                FRAME_BUFFER_GET(x, y).ch = line_buffer[y][x];
                x++;
            }
            while (x < width) {
                FRAME_BUFFER_GET(x, y).ch = ' ';
                x++;
            }
        }
    }

#undef FRAME_BUFFER_GET
}

void ed_draw_frame(void) {
    console_char_t * frame_buffer = console_frame_buffer();
    uint32_t width = console_width();
    uint32_t height = console_height();

#define FRAME_BUFFER_GET(x, y) (frame_buffer[(y) * width + (x)])

    for (uint32_t y = 0; y < height; y++) {
        if (y >= line_count) {
            for (uint32_t x = 0; x < width; x++) FRAME_BUFFER_GET(x, y).ch = ' ';
        }
        else {
            uint32_t x = 0;
            while (line_buffer[y][x] != '\0' && x < width) {
                FRAME_BUFFER_GET(x, y).ch = line_buffer[y][x];
                x++;
            }
            while (x < width) {
                FRAME_BUFFER_GET(x, y).ch = ' ';
                x++;
            }
        }
    }

    for (uint32_t y = 0; y < height; y++) {
        for (uint32_t x = 0; x < width; x++) {
            if (y == cur_y && x == cur_x) FRAME_BUFFER_GET(x, y).color = CONSOLE_COLOR(CONSOLE_COLOR_BLACK, CONSOLE_COLOR_WHITE);
            else FRAME_BUFFER_GET(x, y).color = CONSOLE_COLOR(CONSOLE_COLOR_WHITE, CONSOLE_COLOR_BLACK);
        }
    }

#undef FRAME_BUFFER_GET
}

void ed_save_buffer(void) {
    for (uint32_t i = 0; i < line_count; i++) {
        append_file(KERNEL_LBA_START, file, line_buffer[i], strlen(line_buffer[i]));
        append_file(KERNEL_LBA_START, file, "\n", 1);
    }
}

__CDECL uint32_t command_ed(uint32_t argc, const char ** argv) {
    if (argc != 2) {
        console_print("Invalid arguments. Expected 1\n");

        return 1;
    }

    shell_ready_to_execute = true;

    const char * file_path = argv[1];

    directory_t root_dir = open_filesystem(KERNEL_LBA_START);

    file = open_file_path(root_dir, file_path);

    if (file == 0) {
        console_print("Unable to open file with path \"");
        console_print(file_path);
        console_print("\"\n");

        return 2;
    }

    file_stat_result_t file_stat;
    stat_file(&file_stat, file);

    if (file_stat.type != FILESYSTEM_PAGE_TYPE_FILE) {
        console_print("Invalid filetype for editing\n");

        return 3;
    }

    line_count = 0;
    uint32_t line_capacity = 1;
    uint32_t cur_size = 0;
    uint32_t cur_capacity = 1;

    line_buffer = heap_alloc(line_capacity * sizeof(char *));
    line_buffer[0] = heap_alloc(cur_capacity * sizeof(char));
    line_capacities = heap_alloc(line_capacity * sizeof(uint32_t));
    char * file_buffer = heap_alloc(file_stat.size);

    {
        file_reader_t file_reader;
        file_reader_init(&file_reader, file);
        file_reader_read(&file_reader, file_buffer, file_stat.size);
    }

    for (uint32_t i = 0; file_buffer[i] != '\0'; i++) {
        if (file_buffer[i] != '\n') {
            line_buffer[line_count][cur_size] = file_buffer[i];
            cur_size++;
        }

        if (cur_size == cur_capacity) {
            uint32_t ori_cap = cur_capacity;
            cur_capacity *= 2;
            char * new_buffer = heap_alloc(cur_capacity * sizeof(char));
            memcpy(new_buffer, line_buffer[line_count], ori_cap * sizeof(char));
            heap_free(line_buffer[line_count]);
            line_buffer[line_count] = new_buffer;
        }

        if (file_buffer[i] == '\n') {
            line_buffer[line_count][cur_size] = '\0';

            line_capacities[line_count] = cur_capacity;

            cur_size = 0;
            cur_capacity = 1;

            line_count++;
        }

        if (line_count == line_capacity) {
            uint32_t ori_cap = line_capacity;
            line_capacity *= 2;

            char ** new_buffer = heap_alloc(line_capacity * sizeof(char *));
            memcpy(new_buffer, line_buffer, ori_cap * sizeof(char *));
            heap_free(line_buffer);
            line_buffer = new_buffer;

            uint32_t * new_cap_buffer = heap_alloc(line_capacity * sizeof(uint32_t *));
            memcpy(new_cap_buffer, line_capacities, ori_cap * sizeof(uint32_t *));
            heap_free(line_capacities);
            line_capacities = new_cap_buffer;

            for (uint32_t j = ori_cap; j < line_capacity; j++) line_buffer[j] = heap_alloc(sizeof(char));
        }
    }

    heap_free(file_buffer);

    shell_enable_input_write_back = false;

    ed_init_frame();

    cur_x = 0;
    cur_y = 0;
    frame_pos = 0;

    while (true) {
        ed_draw_frame();

        unsigned char input = keyboard_getch();

        if (keyboard_ctrl_down) {
            if (input == 'x') {
                console_char_t * frame_buffer = console_frame_buffer();
                uint32_t width = console_width();
                uint32_t height = console_height();

                const char * str = "Are you sure? (y/n)";

                for (uint32_t i = 0; i < 19; i++) {
                    frame_buffer[width * (height - 1) + i].ch = str[i];
                }

                char yn_in = keyboard_getch();
                if (yn_in == 'y') break;
            }
            else if (input == 's') {
                ed_save_buffer();
            }
        }
        else {
            switch (input) {
                case KC_DOWN_ARROW:
                    if (cur_y < line_count - 1) {
                        cur_y++;

                        uint32_t line_len = strlen(line_buffer[cur_y]);
                        if (cur_x > line_len) cur_x = line_len;
                    }
                    break;
                case KC_UP_ARROW:
                    if (cur_y > 0) {
                        cur_y--;

                        uint32_t line_len = strlen(line_buffer[cur_y]);
                        if (cur_x > line_len) cur_x = line_len;
                    }
                    break;
                case KC_RIGHT_ARROW:
                    if (cur_x < console_width() - 1 && cur_x < strlen(line_buffer[cur_y])) cur_x++;
                    break;
                case KC_LEFT_ARROW:
                    if (cur_x > 0) cur_x--;
                    break;

                case KC_BACKSPACE: {
                    if (cur_x > 0) {
                        for (uint32_t i = cur_x - 1; line_buffer[cur_y][i] != '\0'; i++) {
                            line_buffer[cur_y][i] = line_buffer[cur_y][i + 1];
                        }
                        cur_x--;
                    }
                } break;

                default: {
                    if (input >= 32 && input <= 126) {

                    }
                } break;
            }
        }
    }

    shell_ready_to_execute = false;
    shell_enable_input_write_back = true;

    heap_free(file_buffer);

    console_update();

    return 0;
}