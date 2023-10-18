#include <stdlib.h>
#include <stdio.h>

int main() {

    char prog[20] = {
        0xA9,
        0x01,
        0x85,
        0x01,
        0xA2,
        0x01,
        0xB5,
        0xFF,
        0x75,
        0x00,
        0x30,
        0x06,
        0x95,
        0x01,
        0xE8,
        0x4C,
        0x06,
        0x02,
        0x95,
        0x01
    };

    FILE *fp = fopen("prog.nes", "wb");
    fwrite(prog, 1, 20, fp);
    fclose(fp);
    free(prog);
}
