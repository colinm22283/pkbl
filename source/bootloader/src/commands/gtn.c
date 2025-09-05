#include <commands/gtn.h>

#include <console/print.h>
#include <console/print_dec.h>
#include <console/newline.h>

#include <keyboard/readline.h>

#include <string/stoi.h>

__CDECL uint32_t command_gtn(uint32_t argc, const char ** argv) {
    if (argc != 4) {
        console_print("Invalid arguments\nExpected 3\n");
        return 1;
    }

    int32_t lower_bound, upper_bound, seed;

    if (stoi(&lower_bound, argv[1])) return 2;
    if (stoi(&upper_bound, argv[2])) return 3;
    if (stoi(&seed, argv[3])) return 4;

    uint32_t random_number = 283214;
    for (int i = 0; i < seed; i++) {
        random_number ^= random_number << 13;
        random_number ^= random_number >> 7;
        random_number ^= random_number << 17;
    }

    random_number = random_number % (upper_bound - lower_bound + 1);
    random_number += lower_bound;

    uint32_t guess = 0;
    do {
        console_print("Enter guess: ");
        char guess_buffer[256];
        keyboard_readline(guess_buffer, 256);
        console_newline();

        int32_t temp;
        if (stoi(&temp, guess_buffer)) continue;
        guess = temp;

        if (guess < random_number) console_print("Too low!\n");
        else if (guess > random_number) console_print("Too high!\n");
    }
    while (guess != random_number);

    console_print("Correct, the number was ");
    console_print_dec(random_number);
    console_print("!\n");

    return 0;
}