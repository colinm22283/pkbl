#include <string/itoa.h>

#include <console/print.h>

uint32_t itoa(char * buffer, int32_t num) {
    uint8_t i = 0;
    uint8_t j = 0;

    if (num < 0) {
        buffer[i] = '-';
        num = -num;
        i++;
        j++;
    }

    if (num == 0) {
        buffer[i] = '0';
        return i + 1;
    }

    for (; num != 0; i++) {
        buffer[i] = (char) ((num % 10) + '0');

        num /= 10;
    }

    uint8_t length = i;
    for (i--; j < i; j++, i--) {
        char temp = buffer[i];
        buffer[i] = buffer[j];
        buffer[j] = temp;
    }



    return length;
}