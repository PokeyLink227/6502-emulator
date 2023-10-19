#include <stdio.h>
#include "6502.h"

typedef unsigned char byte;

typedef struct instruction {
    byte opcode, addr_mode, cycles;
} Instr;

Instr opcode_lookup[0x100] = {
    {OP_BRK, ADR_IMP, 7}, {OP_ORA, ADR_INX, 6}, {OP_XXX, ADR_XXX, 1}, {OP_XXX, ADR_XXX, 1}, {OP_XXX, ADR_XXX, 1}, {OP_ORA, ADR_ZPG, 3}, {OP_ASL, ADR_ZPG, 5}, {OP_XXX, ADR_XXX, 1}, {OP_PHP, ADR_IMP, 3}, {OP_ORA, ADR_IMM, 2}, {OP_ASL, ADR_ACC, 2}, {OP_XXX, ADR_XXX, 1}, {OP_XXX, ADR_XXX, 1}, {OP_ORA, ADR_ABS, 4}, {OP_ASL, ADR_ABS, 6}, {OP_XXX, ADR_XXX, 1},
    {OP_BPL, ADR_REL, 2}, {OP_ORA, ADR_INY, 5}, {OP_XXX, ADR_XXX, 1}, {OP_XXX, ADR_XXX, 1}, {OP_XXX, ADR_XXX, 1}, {OP_ORA, ADR_ZPX, 4}, {OP_ASL, ADR_ZPX, 6}, {OP_XXX, ADR_XXX, 1}, {OP_CLC, ADR_IMP, 2}, {OP_ORA, ADR_ABY, 4}, {OP_XXX, ADR_XXX, 1}, {OP_XXX, ADR_XXX, 1}, {OP_XXX, ADR_XXX, 1}, {OP_ORA, ADR_ABX, 4}, {OP_ASL, ADR_ABX, 7}, {OP_XXX, ADR_XXX, 1},
    {OP_JSR, ADR_ABS, 6}, {OP_AND, ADR_INX, 6}, {OP_XXX, ADR_XXX, 1}, {OP_XXX, ADR_XXX, 1}, {OP_BIT, ADR_ZPG, 3}, {OP_AND, ADR_ZPG, 3}, {OP_ROL, ADR_ZPG, 5}, {OP_XXX, ADR_XXX, 1}, {OP_PLP, ADR_IMP, 4}, {OP_AND, ADR_IMM, 2}, {OP_ROL, ADR_ACC, 2}, {OP_XXX, ADR_XXX, 1}, {OP_BIT, ADR_ABS, 4}, {OP_AND, ADR_ABS, 4}, {OP_ROL, ADR_ABS, 6}, {OP_XXX, ADR_XXX, 1},
    {OP_BMI, ADR_REL, 2}, {OP_AND, ADR_INY, 5}, {OP_XXX, ADR_XXX, 1}, {OP_XXX, ADR_XXX, 1}, {OP_XXX, ADR_XXX, 1}, {OP_AND, ADR_ZPX, 4}, {OP_ROL, ADR_ZPX, 6}, {OP_XXX, ADR_XXX, 1}, {OP_SEC, ADR_IMP, 2}, {OP_AND, ADR_ABY, 4}, {OP_XXX, ADR_XXX, 1}, {OP_XXX, ADR_XXX, 1}, {OP_XXX, ADR_XXX, 1}, {OP_AND, ADR_ABX, 4}, {OP_ROL, ADR_ABX, 7}, {OP_XXX, ADR_XXX, 1},
    {OP_RTI, ADR_IMP, 6}, {OP_EOR, ADR_INX, 6}, {OP_XXX, ADR_XXX, 1}, {OP_XXX, ADR_XXX, 1}, {OP_XXX, ADR_XXX, 1}, {OP_EOR, ADR_ZPG, 3}, {OP_LSR, ADR_ZPG, 5}, {OP_XXX, ADR_XXX, 1}, {OP_PHA, ADR_IMP, 3}, {OP_EOR, ADR_IMM, 2}, {OP_LSR, ADR_ACC, 2}, {OP_XXX, ADR_XXX, 1}, {OP_JMP, ADR_ABS, 3}, {OP_EOR, ADR_ABS, 4}, {OP_LSR, ADR_ABS, 6}, {OP_XXX, ADR_XXX, 1},
    {OP_BVC, ADR_REL, 2}, {OP_EOR, ADR_INY, 5}, {OP_XXX, ADR_XXX, 1}, {OP_XXX, ADR_XXX, 1}, {OP_XXX, ADR_XXX, 1}, {OP_EOR, ADR_ZPX, 4}, {OP_LSR, ADR_ZPX, 6}, {OP_XXX, ADR_XXX, 1}, {OP_CLI, ADR_IMP, 2}, {OP_EOR, ADR_ABY, 4}, {OP_XXX, ADR_XXX, 1}, {OP_XXX, ADR_XXX, 1}, {OP_XXX, ADR_XXX, 1}, {OP_EOR, ADR_ABX, 4}, {OP_LSR, ADR_ABX, 7}, {OP_XXX, ADR_XXX, 1},
    {OP_RTS, ADR_IMP, 6}, {OP_ADC, ADR_INX, 6}, {OP_XXX, ADR_XXX, 1}, {OP_XXX, ADR_XXX, 1}, {OP_XXX, ADR_XXX, 1}, {OP_ADC, ADR_ZPG, 3}, {OP_ROR, ADR_ZPG, 5}, {OP_XXX, ADR_XXX, 1}, {OP_PLA, ADR_IMP, 4}, {OP_ADC, ADR_IMM, 2}, {OP_ROR, ADR_ACC, 2}, {OP_XXX, ADR_XXX, 1}, {OP_JMP, ADR_IND, 5}, {OP_ADC, ADR_ABS, 4}, {OP_ROR, ADR_ABS, 6}, {OP_XXX, ADR_XXX, 1},
    {OP_BVS, ADR_REL, 2}, {OP_ADC, ADR_INY, 5}, {OP_XXX, ADR_XXX, 1}, {OP_XXX, ADR_XXX, 1}, {OP_XXX, ADR_XXX, 1}, {OP_ADC, ADR_ZPX, 4}, {OP_ROR, ADR_ZPX, 6}, {OP_XXX, ADR_XXX, 1}, {OP_SEI, ADR_IMP, 2}, {OP_ADC, ADR_ABY, 4}, {OP_XXX, ADR_XXX, 1}, {OP_XXX, ADR_XXX, 1}, {OP_XXX, ADR_XXX, 1}, {OP_ADC, ADR_ABX, 4}, {OP_ROR, ADR_ABX, 7}, {OP_XXX, ADR_XXX, 1},
    {OP_XXX, ADR_XXX, 1}, {OP_STA, ADR_INX, 6}, {OP_XXX, ADR_XXX, 1}, {OP_XXX, ADR_XXX, 1}, {OP_STY, ADR_ZPG, 3}, {OP_STA, ADR_ZPG, 3}, {OP_STX, ADR_ZPG, 3}, {OP_XXX, ADR_XXX, 1}, {OP_DEY, ADR_IMP, 2}, {OP_XXX, ADR_XXX, 1}, {OP_TXA, ADR_IMP, 2}, {OP_XXX, ADR_XXX, 1}, {OP_STY, ADR_ABS, 4}, {OP_STA, ADR_ABS, 4}, {OP_STX, ADR_ABS, 4}, {OP_XXX, ADR_XXX, 1},
    {OP_BCC, ADR_REL, 2}, {OP_STA, ADR_INY, 6}, {OP_XXX, ADR_XXX, 1}, {OP_XXX, ADR_XXX, 1}, {OP_STY, ADR_ZPX, 4}, {OP_STA, ADR_ZPX, 4}, {OP_STX, ADR_ZPY, 4}, {OP_XXX, ADR_XXX, 1}, {OP_TYA, ADR_IMP, 2}, {OP_STA, ADR_ABY, 5}, {OP_TXS, ADR_IMP, 2}, {OP_XXX, ADR_XXX, 1}, {OP_XXX, ADR_XXX, 1}, {OP_STA, ADR_ABX, 4}, {OP_XXX, ADR_XXX, 1}, {OP_XXX, ADR_XXX, 1},
    {OP_LDY, ADR_IMM, 2}, {OP_LDA, ADR_INX, 6}, {OP_LDX, ADR_IMM, 2}, {OP_XXX, ADR_XXX, 1}, {OP_LDY, ADR_ZPG, 3}, {OP_LDA, ADR_ZPG, 3}, {OP_LDX, ADR_ZPG, 3}, {OP_XXX, ADR_XXX, 1}, {OP_TAY, ADR_IMP, 2}, {OP_LDA, ADR_IMM, 2}, {OP_TAX, ADR_IMP, 2}, {OP_XXX, ADR_XXX, 1}, {OP_LDY, ADR_ABS, 4}, {OP_LDA, ADR_ABS, 4}, {OP_LDX, ADR_ABS, 4}, {OP_XXX, ADR_XXX, 1},
    {OP_BCS, ADR_REL, 2}, {OP_LDA, ADR_INY, 5}, {OP_XXX, ADR_XXX, 1}, {OP_XXX, ADR_XXX, 1}, {OP_LDY, ADR_ZPX, 3}, {OP_LDA, ADR_ZPX, 4}, {OP_LDX, ADR_ZPY, 4}, {OP_XXX, ADR_XXX, 1}, {OP_CLV, ADR_IMP, 2}, {OP_LDA, ADR_ABY, 4}, {OP_TSX, ADR_IMP, 2}, {OP_XXX, ADR_XXX, 1}, {OP_LDY, ADR_ABX, 4}, {OP_LDA, ADR_ABX, 4}, {OP_ADC, ADR_ABY, 4}, {OP_XXX, ADR_XXX, 1},
    {OP_CPY, ADR_IMM, 2}, {OP_CMP, ADR_INX, 6}, {OP_XXX, ADR_XXX, 1}, {OP_XXX, ADR_XXX, 1}, {OP_CPY, ADR_ZPG, 3}, {OP_CMP, ADR_ZPG, 3}, {OP_DEC, ADR_ZPG, 5}, {OP_XXX, ADR_XXX, 1}, {OP_INY, ADR_IMP, 2}, {OP_CMP, ADR_IMM, 2}, {OP_DEX, ADR_IMP, 2}, {OP_XXX, ADR_XXX, 1}, {OP_CPY, ADR_ABS, 4}, {OP_CMP, ADR_ABS, 4}, {OP_DEC, ADR_ABS, 6}, {OP_XXX, ADR_XXX, 1},
    {OP_BNE, ADR_REL, 2}, {OP_CMP, ADR_INY, 5}, {OP_XXX, ADR_XXX, 1}, {OP_XXX, ADR_XXX, 1}, {OP_XXX, ADR_XXX, 1}, {OP_CMP, ADR_ZPX, 4}, {OP_DEC, ADR_ZPX, 6}, {OP_XXX, ADR_XXX, 1}, {OP_CLD, ADR_IMP, 2}, {OP_CMP, ADR_ABY, 4}, {OP_XXX, ADR_XXX, 1}, {OP_XXX, ADR_XXX, 1}, {OP_XXX, ADR_XXX, 1}, {OP_CMP, ADR_ABX, 4}, {OP_DEC, ADR_ABX, 7}, {OP_XXX, ADR_XXX, 1},
    {OP_CPX, ADR_IMM, 2}, {OP_SBC, ADR_INX, 6}, {OP_XXX, ADR_XXX, 1}, {OP_XXX, ADR_XXX, 1}, {OP_CPX, ADR_ZPG, 3}, {OP_SBC, ADR_ZPG, 3}, {OP_INC, ADR_ZPG, 5}, {OP_XXX, ADR_XXX, 1}, {OP_INX, ADR_IMP, 2}, {OP_SBC, ADR_IMM, 2}, {OP_NOP, ADR_IMP, 2}, {OP_XXX, ADR_XXX, 1}, {OP_CPX, ADR_ABS, 4}, {OP_SBC, ADR_ABS, 4}, {OP_INC, ADR_ABS, 6}, {OP_XXX, ADR_XXX, 1},
    {OP_BEQ, ADR_REL, 2}, {OP_SBC, ADR_INY, 5}, {OP_XXX, ADR_XXX, 1}, {OP_XXX, ADR_XXX, 1}, {OP_XXX, ADR_XXX, 1}, {OP_SBC, ADR_ZPX, 4}, {OP_INC, ADR_ZPX, 6}, {OP_XXX, ADR_XXX, 1}, {OP_SED, ADR_IMP, 2}, {OP_SBC, ADR_ABY, 4}, {OP_XXX, ADR_XXX, 1}, {OP_XXX, ADR_XXX, 1}, {OP_XXX, ADR_XXX, 1}, {OP_SBC, ADR_ABX, 4}, {OP_INC, ADR_ABX, 7}, {OP_XXX, ADR_XXX, 1}
};

unsigned short pc;
byte acc, x, y, stkptr, status;
byte ram[0x0800];

Instr cur_instr;
unsigned short data_adr;
byte cycles;

void set_flag(byte flag, unsigned short b) {
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

byte read(unsigned short adr) {
    if (adr < 0x2000) {
        /*read ram*/
        return ram[adr % 0x0800]; /* could be optimized if deemed necessary*/
    } else if (adr < 0x4000) {
        /* read PPU registers   (adr - 0x2000) % 0x0008 + 0x2000*/
    } else if (adr < 0x4018) {
    }
    return 0x00;
}

byte write(unsigned short adr, byte data) {
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
    unsigned short low, high;
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
        unsigned short sum = acc + read(data_adr) + get_flag(CARRY);

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
        unsigned short temp = acc - read(data_adr);
        set_flag(CARRY, acc >= read(data_adr));
        set_flag(NEGATIVE, temp & 0x80);
        set_flag(ZERO, temp == 0);
        return 0;
    }

    case OP_CPX: {
        unsigned short temp = x - read(data_adr);
        set_flag(CARRY, x >= read(data_adr));
        set_flag(NEGATIVE, temp & 0x80);
        set_flag(ZERO, temp == 0);
        return 0;
    }

    case OP_CPY: {
        unsigned short temp = y - read(data_adr);
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
        unsigned short temp = acc - read(data_adr) - (1 - get_flag(CARRY));
        unsigned short temp = acc + (~read(data_adr) + 1) - 1 + get_flag(CARRY);
        */
        unsigned short temp = acc + ~read(data_adr) + get_flag(CARRY);

        set_flag(CARRY, !(temp & 0xFF00));
        set_flag(ZERO, temp & 0x00FF == 0x00);
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
    printf("acc:[%02X]  x:[%02X]  y:[%02X]  stkptr:[%02X]  pc:[%04X]  status:[N:%c V:%c - B:%c D:%c I:%c Z:%c C:%c]\n",
        acc, x, y, stkptr, pc,
        status & NEGATIVE ? '1' : '0',
        status & OVERFLOW ? '1' : '0',
        status & BRKCOMMAND ? '1' : '0',
        status & DECIMALMODE ? '1' : '0',
        status & IRQDISABLE ? '1' : '0',
        status & ZERO ? '1' : '0',
        status & CARRY ? '1' : '0'
    );
    for (int i = 0; i < 0xFF; i++) printf("%02X ", ram[i]);
    printf("\n");
}

byte reset_cpu() {
    acc = 0x00;
    x = 0x00;
    y = 0x00;
    stkptr = 0xFF;
    pc = 0x200;
    status = UNUSED;
}

byte load_rom(const char *file_name, unsigned short addr) {
    int ret_size, f_size;
    FILE *fp = fopen(file_name, "rb");
    if (!fp) return 0;

    fseek(fp, 0L, SEEK_END);
    f_size = ftell(fp);
    fseek(fp, 0L, SEEK_SET);

    ret_size = fread(ram + addr, 1, f_size, fp);
    if (ret_size != f_size) return 0;

    fclose(fp);
    return 1;
}

int main(int argc, char **argv) {
    byte extra_cycle;

    load_rom("prog.nes", 0x0200);
    pc = 0x0200;

    reset_cpu();
    disp_cpu();

    while (cur_instr.opcode != OP_BRK) {
        cur_instr = opcode_lookup[read(pc++)];
        cycles = cur_instr.cycles;
        printf("%s\n", mnemonics[cur_instr.opcode]);
        extra_cycle = set_address_mode(cur_instr.addr_mode);
        extra_cycle &= execute_instr(cur_instr.opcode);
        if (extra_cycle) cycles++;
    }

    disp_cpu();


    return 0;
}
