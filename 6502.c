#include <stdio.h>
#include "6502.h"
#include "color.h"

word pc;
byte acc, x, y, stkptr, status;
byte ram[0x10000];

Instr cur_instr;
word data_adr;
byte cycles;

void set_flag(byte flag, word b) {
    if (b) {
        status |= flag;
    } else {
        status &= ~flag;
    }
    return;
}

byte get_flag(byte flag) {
    return (status & flag) != 0;
}

byte read(word adr) {

    return ram[adr];



    if (adr < 0x2000) {
        /*read ram*/
        return ram[adr % 0x0800]; /* could be optimized if deemed necessary*/
    } else if (adr < 0x4000) {
        /* read PPU registers   (adr - 0x2000) % 0x0008 + 0x2000*/
    } else if (adr < 0x4018) {
    }
    return 0x00;
}

byte write(word adr, byte data) {
    if (adr < 0x2000) {
        /*write to ram*/
        ram[adr % 0x0800] = data;
        return 0x01;
    } else if (adr < 0x4000) {
        /* write to PPU registers   (adr - 0x2000) % 0x0008 + 0x2000*/
    } else if (adr < 0x4018) {
    }
    return 0x00;
}

byte set_address_mode(byte mode) {
    word low, high;
    switch (mode) {
    case ADR_XXX: {
        return 0;
    }

    case ADR_ACC: {
        return 0;
    }

    case ADR_IMM: {
        data_adr = pc++;
        return 0;
    }

    case ADR_ZPG: {
        data_adr = read(pc++);
        return 0;
    }

    case ADR_ZPX: {
        data_adr = (x + read(pc++)) & 0x00FF;
        return 0;
    }

    case ADR_ZPY: {
        data_adr = (y + read(pc++)) & 0x00FF;
        return 0;
    }

    case ADR_ABS: {
        low = read(pc++);
        high = read(pc++);
        data_adr = (high << 8) | low;
        return 0;
    }

    case ADR_ABX: {
        low = read(pc++);
        high = read(pc++);
        data_adr = (high << 8) | low;
        data_adr += x;
        /* checks if page boundry was crossed */
        return ((data_adr & 0xFF00) != (high << 8));
    }

    case ADR_ABY: {
        low = read(pc++);
        high = read(pc++);
        data_adr = (high << 8) | low;
        data_adr += y;
        return ((data_adr & 0xFF00) != (high << 8));
    }

    case ADR_IMP: {
        return 0;
    }

    case ADR_REL: { /* additional cycles need to be calculated per instruction */
        data_adr = read(pc++);
        if (data_adr & 0x80) data_adr |= 0xFF00;
        return 0;
    }

    case ADR_INX: {
        low = read(pc++);
        low = (low + x) & 0x00FF;
        data_adr = low;

        low = read(data_adr++);
        high = read(data_adr);
        data_adr = (high << 8) | low;
        return 0;
    }

    case ADR_INY: {
        low = read(pc++);
        high = read(pc++);
        data_adr = (high << 8) | low;
        data_adr += y;
        return ((data_adr & 0xFF00) != (high << 8));
    }

    case ADR_IND: {
        low = read(pc++);
        high = read(pc++);
        data_adr = (high << 8) | low;

        /*
            needed in order to emulate a bug that caused invalid memory addresses to be read
            when reading across page boundries wrap rather than cross
        */
        if (low == 0x00FF) {
            low = read(data_adr);
            high = read(data_adr & 0xFF00);
            data_adr = (high << 8) | low;
        } else {
            low = read(data_adr++);
            high = read(data_adr);
            data_adr = (high << 8) | low;
        }
        return 0;
    }

    }

    return 0; // num extra cycles
}

byte execute_instr(byte instr) {
    switch (instr) {
    case OP_ADC: {
        if (get_flag(DECIMALMODE)) printf("AHHHHHHHH I CANT DO THIS\n");
        word sum = acc + read(data_adr) + get_flag(CARRY);

        set_flag(OVERFLOW, (~(acc ^ read(data_adr)) & (acc ^ (byte)sum)) & 0x0080);
        set_flag(CARRY, sum > 0xFF);
        set_flag(ZERO, sum == 0x00);
        set_flag(NEGATIVE, sum & 0x80);

        acc = sum & 0x00FF;
        return 1;
    }

    case OP_AND: {
        acc &= read(data_adr);
        set_flag(ZERO, acc == 0x00);
        set_flag(NEGATIVE, acc & 0x80);
        return 1;
    }

    case OP_ASL: {
        if (cur_instr.addr_mode == ADR_ACC) {
            set_flag(CARRY, acc & 0x80);
            acc <<= 1;
            set_flag(ZERO, acc == 0x00);
            set_flag(NEGATIVE, acc & 0x80);
        } else {
            byte temp = read(data_adr);
            set_flag(CARRY, temp & 0x80);
            temp <<= 1;
            set_flag(ZERO, temp == 0x00);
            set_flag(NEGATIVE, temp & 0x80);
        }
        return 0;
    }

    case OP_BCC: {
        if (!get_flag(CARRY)) {
            cycles++;
            if ((pc & 0xFF00) != ((pc + data_adr) & 0xFF00)) cycles++;
            pc += data_adr;
        }
        return 0;
    }

    case OP_BCS: {
        if (get_flag(CARRY)) {
            cycles++;
            if ((pc & 0xFF00) != ((pc + data_adr) & 0xFF00)) cycles++;
            pc += data_adr;
        }
        return 0;
    }

    case OP_BEQ: {
        if (get_flag(ZERO)) {
            cycles++;
            if ((pc & 0xFF00) != ((pc + data_adr) & 0xFF00)) cycles++;
            pc += data_adr;
        }
        return 0;
    }

    case OP_BIT: {
        byte temp = read(data_adr);
        set_flag(ZERO, (temp & acc) == 0);
        set_flag(OVERFLOW, temp & 0x40);
        set_flag(NEGATIVE, temp & 0x80);
        return 0;
    }

    case OP_BMI: {
        if (get_flag(NEGATIVE)) {
            cycles++;
            if ((pc & 0xFF00) != ((pc + data_adr) & 0xFF00)) cycles++;
            pc += data_adr;
        }
        return 0;
    }

    case OP_BNE: {
        if (!get_flag(ZERO)) {
            cycles++;
            if ((pc & 0xFF00) != ((pc + data_adr) & 0xFF00)) cycles++;
            pc += data_adr;
        }
        return 0;
    }

    case OP_BPL: {
        if (!get_flag(NEGATIVE)) {
            cycles++;
            if ((pc & 0xFF00) != ((pc + data_adr) & 0xFF00)) cycles++;
            pc += data_adr;
        }
        return 0;
    }

    case OP_BRK: {
        write(0x0100 + stkptr--, (pc >> 8) & 0x00FF);
        write(0x0100 + stkptr--, pc & 0x00FF);
        write(0x0100 + stkptr--, status);

        pc = read(0xFFFE);
        pc |= (read(0xFFFF) << 8) & 0xFF00;

        set_flag(BRKCOMMAND, 1);
        return 0;
    }

    case OP_BVC: {
        if (!get_flag(OVERFLOW)) {
            cycles++;
            if ((pc & 0xFF00) != ((pc + data_adr) & 0xFF00)) cycles++;
            pc += data_adr;
        }
        return 0;
    }

    case OP_BVS: {
        if (get_flag(OVERFLOW)) {
            cycles++;
            if ((pc & 0xFF00) != ((pc + data_adr) & 0xFF00)) cycles++;
            pc += data_adr;
        }
        return 0;
    }

    case OP_CLC: {
        status &= ~CARRY;
        return 0;
    }

    case OP_CLD: {
        status &= ~DECIMALMODE;
        return 0;
    }

    case OP_CLI: {
        status &= ~IRQDISABLE;
        return 0;
    }

    case OP_CLV: {
        status &= ~OVERFLOW;
        return 0;
    }

    case OP_CMP: {
        word temp = acc - read(data_adr);
        set_flag(CARRY, acc >= read(data_adr));
        set_flag(NEGATIVE, temp & 0x80);
        set_flag(ZERO, temp == 0);
        return 0;
    }

    case OP_CPX: {
        word temp = x - read(data_adr);
        set_flag(CARRY, x >= read(data_adr));
        set_flag(NEGATIVE, temp & 0x80);
        set_flag(ZERO, temp == 0);
        return 0;
    }

    case OP_CPY: {
        word temp = y - read(data_adr);
        set_flag(CARRY, y >= read(data_adr));
        set_flag(NEGATIVE, temp & 0x80);
        set_flag(ZERO, temp == 0);
        return 0;
    }

    case OP_DEC: {
        byte temp = read(data_adr);
        temp -= 1;
        write(data_adr, temp);
        set_flag(ZERO, temp == 0x00);
        set_flag(NEGATIVE, temp & 0x80);
        return 0;
    }

    case OP_DEX: {
        x -= 1;
        set_flag(ZERO, x == 0x00);
        set_flag(NEGATIVE, x & 0x80);
        return 0;
    }

    case OP_DEY: {
        y -= 1;
        set_flag(ZERO, y == 0x00);
        set_flag(NEGATIVE, y & 0x80);
        return 0;
    }

    case OP_EOR: {
        acc ^= read(data_adr);
        set_flag(ZERO, acc == 0x00);
        set_flag(NEGATIVE, acc & 0x80);
        return 1;
    }

    case OP_INC: {
        byte temp = read(data_adr);
        temp += 1;
        write(data_adr, temp);
        set_flag(ZERO, temp == 0x00);
        set_flag(NEGATIVE, temp & 0x80);
        return 0;
    }

    case OP_INX: {
        x += 1;
        set_flag(ZERO, x == 0x00);
        set_flag(NEGATIVE, x & 0x80);
        return 0;
    }

    case OP_INY: {
        y -= 1;
        set_flag(ZERO, y == 0x00);
        set_flag(NEGATIVE, y & 0x80);
        return 0;
    }

    case OP_JMP: {
        pc = data_adr;
        return 0;
    }

    case OP_JSR: {
        pc--;
        write(0x0100 + stkptr--, (pc >> 8) & 0x00FF);
        write(0x0100 + stkptr--, pc & 0x00FF);
        pc = data_adr;
        return 0;
    }

    case OP_LDA: {
        acc = read(data_adr);
        set_flag(ZERO, acc == 0x00);
        set_flag(NEGATIVE, acc & 0x80);
        return 0;
    }

    case OP_LDX: {
        x = read(data_adr);
        set_flag(ZERO, x == 0x00);
        set_flag(NEGATIVE, x & 0x80);
        return 0;
    }

    case OP_LDY: {
        y = read(data_adr);
        set_flag(ZERO, y == 0x00);
        set_flag(NEGATIVE, y & 0x80);
        return 0;
    }

    case OP_LSR: {
        if (cur_instr.addr_mode == ADR_ACC) {
            set_flag(CARRY, acc & 0x80);
            acc >>= 1;
            set_flag(ZERO, acc == 0x00);
            set_flag(NEGATIVE, acc & 0x80);
        } else {
            byte temp = read(data_adr);
            set_flag(CARRY, temp & 0x01);
            temp >>= 1;
            set_flag(ZERO, temp == 0x00);
            set_flag(NEGATIVE, 0);
        }
        return 0;
    }

    case OP_NOP: {
        /*implement unofficial NOP's in lookup table*/
        return 0;
    }

    case OP_ORA: {
        acc |= read(data_adr);
        set_flag(ZERO, acc == 0x00);
        set_flag(NEGATIVE, acc & 0x80);
        return 0;
    }

    case OP_PHA: {
        write(0x0100 | stkptr, acc);
        stkptr--;
        return 0;
    }

    case OP_PHP: {
        write(0x0100 | stkptr, status);
        stkptr--;
        return 0;
    }

    case OP_PLA: {
        stkptr++;
        acc = read(0x0100 | stkptr);
        return 0;
    }

    case OP_PLP: {
        stkptr++;
        status = read(0x0100 | stkptr);
        return 0;
    }

    case OP_ROL: {
        byte temp;
        if (cur_instr.addr_mode == ADR_ACC) {
            temp = acc << 1;
            if (get_flag(CARRY)) temp |= 0x01;
            set_flag(CARRY, acc & 0x80);
            acc = temp;
        }
        else {
            temp = read(data_adr) << 1;
            if (get_flag(CARRY)) temp |= 0x01;
            set_flag(CARRY, read(data_adr) & 0x80);
            write(data_adr, temp);
        }
        set_flag(ZERO, temp == 0x00);
        set_flag(NEGATIVE, temp & 0x80);
        return 0;
    }

    case OP_ROR: {
        byte temp;
        if (cur_instr.addr_mode == ADR_ACC) {
            temp = acc >> 1;
            if (get_flag(CARRY)) temp |= 0x80;
            set_flag(CARRY, acc & 0x01);
            acc = temp;
        }
        else {
            temp = read(data_adr) >> 1;
            if (get_flag(CARRY)) temp |= 0x80;
            set_flag(CARRY, read(data_adr) & 0x01);
            write(data_adr, temp);
        }
        set_flag(ZERO, temp == 0x00);
        set_flag(NEGATIVE, temp & 0x80);
        return 0;
    }

    case OP_RTI: {
        status = read(++stkptr);
        pc = read(++stkptr);
        pc |= (read(++stkptr) << 8) & 0xFF00;
        return 0;
    }

    case OP_RTS: {
        pc = read(++stkptr);
        pc |= (read(++stkptr) << 8) & 0xFF00;

        pc++;
        return 0;
    }

    case OP_SBC: {
        /*
        word temp = acc - read(data_adr) - (1 - get_flag(CARRY));
        word temp = acc + (~read(data_adr) + 1) - 1 + get_flag(CARRY);
        */
        word temp = acc + ~read(data_adr) + get_flag(CARRY);

        set_flag(CARRY, !(temp & 0xFF00));
        set_flag(ZERO, (temp & 0x00FF) == 0x00);
        set_flag(OVERFLOW, ((acc ^ read(data_adr)) & (acc ^ temp)) & 0x0080);
        set_flag(NEGATIVE, temp & 0x80);
        acc = temp & 0x00FF;
        return 0;
    }

    case OP_SEC: {
        set_flag(CARRY, 1);
        return 0;
    }

    case OP_SED: {
        printf("dont do this\n");
        set_flag(DECIMALMODE, 1);
        return 0;
    }

    case OP_SEI: {
        set_flag(IRQDISABLE, 1);
        return 0;
    }

    case OP_STA: {
        write(data_adr, acc);
        return 0;
    }

    case OP_STX: {
        write(data_adr, x);
        return 0;
    }

    case OP_STY: {
        write(data_adr, y);
        return 0;
    }

    case OP_TAX: {
        x = acc;
        set_flag(ZERO, x == 0x00);
        set_flag(NEGATIVE, x & 0x80);
        return 0;
    }

    case OP_TAY: {
        y = acc;
        set_flag(ZERO, y == 0x00);
        set_flag(NEGATIVE, y & 0x80);
        return 0;
    }

    case OP_TSX: {
        x = stkptr;
        set_flag(ZERO, x == 0x00);
        set_flag(NEGATIVE, x & 0x80);
        return 0;
    }

    case OP_TXA: {
        acc = x;
        set_flag(ZERO, acc == 0x00);
        set_flag(NEGATIVE, acc & 0x80);
        return 0;
    }

    case OP_TXS: {
        stkptr = x;
        return 0;
    }

    case OP_TYA: {
        acc = y;
        set_flag(ZERO, acc == 0x00);
        set_flag(NEGATIVE, acc & 0x80);
        return 0;
    }
    }
    return 0;
}

void disp_cpu() {
    printf("\033[H");
    char line[81];
    for (int i = 0; i < 80; i++) line[i] = 196;

    line[0] = 218;
    line[3] = 'C';
    line[4] = 'P';
    line[5] = 'U';
    line[79] = 191;
    line[80] = '\0';
    printf("%s\n", line);

    printf("%c  ACC:[ %02X ]  X:[ %02X ]  Y:[ %02X ]  StkPtr:[ %02X ]  Status:[ %s %s %s %s %s %s %s %s ]   %c\n",
        179,
        acc, x, y, stkptr,
        status & NEGATIVE ? "\033[92mN\033[39m" : "\033[91mN\033[39m",
        status & OVERFLOW ? "\033[92mV\033[39m" : "\033[91mV\033[39m",
        "\033[92m-\033[39m",
        status & BRKCOMMAND ? "\033[92mB\033[39m" : "\033[91mB\033[39m",
        status & DECIMALMODE ? "\033[92mD\033[39m" : "\033[91mD\033[39m",
        status & IRQDISABLE ? "\033[92mI\033[39m" : "\033[91mI\033[39m",
        status & ZERO ? "\033[92mZ\033[39m" : "\033[91mZ\033[39m",
        status & CARRY ? "\033[92mC\033[39m" : "\033[91mC\033[39m",
        179
    );

    line[0] = 195;
    line[3] = 196;
    line[4] = 196;
    line[5] = 196;
    line[59] = 194;
    line[79] = 180;
    printf("%s\n", line);

    printf("%c >                                                        %c   PC:[ %04X ]     %c\n", 179, 179, pc, 179);

    line[56] = 196;
    line[59] = 197;
    printf("%s", line);
    printf("\033[92m\033[5;11HZERO PAGE\033[39m");
    printf("\033[5;24HSTACK PAGE");
    printf("\033[5;38HPAGE 78");
    printf("\033[5;49HPAGE 23");
    printf("\n");

    //printf("%s\n", line);

    for (int i = 0; i <= 0xF0; i += 0x10) {
        printf("%c  %02X  %c  ", 179, i, 179);
        for (int j = 0; j <= 0x0F; j += 0x01) printf("%02X ", ram[i + j]);
        printf(" %c                   %c\n", 179, 179);
    }

    for (int i = 1; i < 80; i++) line[i] = 196;
    line[0] = 192;
    line[7] = 193;
    line[59] = 193;
    line[79] = 217;
    printf("%s\n", line);

    //for (int i = 0; i < 0xFF; i++) printf("%02X ", ram[i]);
    printf("\033[4;5H");
}

void reset_cpu() {
    acc = 0x00;
    x = 0x00;
    y = 0x00;
    stkptr = 0xFF;
    //pc = 0x200;
    status = UNUSED;
}

byte load_rom(const char *file_name, word addr) {
    int read_size, f_size;
    struct INES_HEADER header;
    FILE *fp = fopen(file_name, "rb");
    if (!fp) return 0;

    read_size = fread(&header, 1, 16, fp);
    if (read_size != 16) return 0;

    if (header.ID[0] != 'N' || header.ID[1] != 'E' || header.ID[2] != 'S' || header.ID[3] != 0x1A) return 0;


    // read trainer
    // read PRG ROM
    // read CHR rom
    // read INST-rom
    // read PROM
    byte mapper = header.FLAGS[1] & 0x0F;
    mapper |= (header.FLAGS[0] & 0xF0) >> 4;
    printf("mapper id: %d\n", mapper);
    return 0;


    /*
    fseek(fp, 0, SEEK_END);
    f_size = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    ret_size = fread(ram + addr, 1, f_size, fp);
    if (ret_size != f_size) return 0;
    */

    fclose(fp);
    return 1;
}

byte load_prog(const char *file_name, word addr) {
    int read_size, f_size;
    FILE *fp = fopen(file_name, "rb");
    if (!fp) return 0;

    fseek(fp, 0, SEEK_END);
    f_size = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    fread(ram + addr, 1, f_size, fp);
    fclose(fp);

    return 0;
}

int main(int argc, char **argv) {
    byte extra_cycle, c;

    init_color();
    printf("loading\n");

    if (load_prog("prog.nes", 0x0200)) printf("loaded rom\n");
    else {
        printf("failed to load rom\n");
        //return 0;
    }



    pc = 0x0200;

    reset_cpu();
    printf("\033[2J\033[H");
    disp_cpu();


    while (1) {

        c = getc(stdin);
        if (c == 'q') break;

        disp_cpu();
        cur_instr = opcode_lookup[read(pc++)];
        cycles = cur_instr.cycles;
        printf("%s", mnemonics[cur_instr.opcode]);
        extra_cycle = set_address_mode(cur_instr.addr_mode);
        extra_cycle &= execute_instr(cur_instr.opcode);
        if (extra_cycle) cycles++;
    }

    printf("\033[2J\033[H");


    return 0;
}
