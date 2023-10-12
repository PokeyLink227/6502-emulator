#include <stdio.h>
#include "6502.h"

typedef unsigned char byte;

struct instruction {
    byte opcode, addr_mode, cycles;
} Instr;

Instr opcode_lookup[0xF][0xF] = {
    {{OP_BRK, ADR_IMPL  , 7}, {OP_ORA, ADR_INDX  , 6}, {OP_XXX, ADR_NONE  , 1}, {OP_XXX, ADR_NONE  , 1}, {OP_XXX, ADR_NONE  , 1}, {OP_ORA, ADR_ZP    , 3}, {OP_ASL, ADR_ZP    , 5}, {OP_XXX, ADR_NONE  , 1}, {OP_PHP, ADR_IMPL  , 3}, {OP_ORA, ADR_IMM   , 2}, {OP_ASL, ADR_ACC   , 2}, {OP_XXX, ADR_NONE  , 1}, {OP_XXX, ADR_NONE  , 1}, {OP_ORA, ADR_ABS   , 4}, {OP_ASL, ADR_ABS   , 6}, {OP_XXX, ADR_NONE  , 1}},
    {{OP_BPL, ADR_REL   , 2}, {OP_ORA, ADR_INDY  , 5}, {OP_XXX, ADR_NONE  , 1}, {OP_XXX, ADR_NONE  , 1}, {OP_XXX, ADR_NONE  , 1}, {OP_ORA, ADR_ZPX   , 4}, {OP_ASL, ADR_APX   , 6}, {OP_XXX, ADR_NONE  , 1}, {OP_CLC, ADR_IMPL  , 2}, {OP_ORA, ADR_ABSY  , 4}, {OP_XXX, ADR_NONE  , 1}, {OP_XXX, ADR_NONE  , 1}, {OP_XXX, ADR_NONE  , 1}, {OP_ORA, ADR_ABSX  , 4}, {OP_ASL, ADR_ABSX  , 7}, {OP_XXX, ADR_NONE  , 1}},
    {{OP_JSR, ADR_ABS   , 6}, {OP_AND, ADR_INDX  , 6}, {OP_XXX, ADR_NONE  , 1}, {OP_XXX, ADR_NONE  , 1}, {OP_BIT, ADR_ZP    , 3}, {OP_AND, ADR_ZP    , 3}, {OP_ROL, ADR_ZP    , 5}, {OP_XXX, ADR_NONE  , 1}, {OP_PLP, ADR_IMPL  , 4}, {OP_AND, ADR_IMM   , 2}, {OP_ROL, ADR_ACC   , 2}, {OP_XXX, ADR_NONE  , 1}, {OP_BIT, ADR_ABS   , 4}, {OP_AND, ADR_ABS   , 4}, {OP_ROL, ADR_ABS   , 6}, {OP_XXX, ADR_NONE  , 1}},
    {{OP_BMI, ADR_REL   , 2}, {OP_AND, ADR_INDY  , 5}, {OP_XXX, ADR_NONE  , 1}, {OP_XXX, ADR_NONE  , 1}, {OP_XXX, ADR_NONE  , 1}, {OP_AND, ADR_ZPX   , 4}, {OP_ROL, ADR_ZPX   , 6}, {OP_XXX, ADR_NONE  , 1}, {OP_SEC, ADR_IMPL  , 2}, {OP_AND, ADR_ABSY  , 4}, {OP_XXX, ADR_NONE  , 1}, {OP_XXX, ADR_NONE  , 1}, {OP_XXX, ADR_NONE  , 1}, {OP_AND, ADR_ABSX  , 4}, {OP_ROL, ADR_ABSX  , 7}, {OP_XXX, ADR_NONE  , 1}},
    {{OP_RTI, ADR_IMPL  , 6}, {OP_EOR, ADR_INDX  , 6}, {OP_XXX, ADR_NONE  , 1}, {OP_XXX, ADR_NONE  , 1}, {OP_XXX, ADR_NONE  , 1}, {OP_EOR, ADR_ZP    , 3}, {OP_LSR, ADR_ZP    , 5}, {OP_XXX, ADR_NONE  , 1}, {OP_PHA, ADR_IMPL  , 3}, {OP_EOR, ADR_IMM   , 2}, {OP_LSR, ADR_ACC   , 2}, {OP_XXX, ADR_NONE  , 1}, {OP_JMP, ADR_ABS   , 3}, {OP_EOR, ADR_ABS   , 4}, {OP_LSR, ADR_ABS   , 6}, {OP_XXX, ADR_NONE  , 1}},
    {{OP_BVC, ADR_REL   , 2}, {OP_EOR, ADR_INDY  , 5}, {OP_XXX, ADR_NONE  , 1}, {OP_XXX, ADR_NONE  , 1}, {OP_XXX, ADR_NONE  , 1}, {OP_EOR, ADR_ZPX   , 4}, {OP_LSR, ADR_ZPX   , 6}, {OP_XXX, ADR_NONE  , 1}, {OP_CLI, ADR_IMPL  , 2}, {OP_EOR, ADR_ABSY  , 4}, {OP_XXX, ADR_NONE  , 1}, {OP_XXX, ADR_NONE  , 1}, {OP_XXX, ADR_NONE  , 1}, {OP_EOR, ADR_ABSX  , 4}, {OP_LSR, ADR_ABSX  , 7}, {OP_XXX, ADR_NONE  , 1}},
    {{OP_RTS, ADR_IMPL  , 6}, {OP_ADC, ADR_INDX  , 6}, {OP_XXX, ADR_NONE  , 1}, {OP_XXX, ADR_NONE  , 1}, {OP_XXX, ADR_NONE  , 1}, {OP_ADC, ADR_ZP    , 3}, {OP_ROR, ADR_ZP    , 5}, {OP_XXX, ADR_NONE  , 1}, {OP_PLA, ADR_IMPL  , 4}, {OP_ADC, ADR_IMM   , 2}, {OP_ROR, ADR_ACC   , 2}, {OP_XXX, ADR_NONE  , 1}, {OP_JMP, ADR_INDABS, 5}, {OP_ADC, ADR_ABS   , 4}, {OP_ROR, ADR_ABS   , 6}, {OP_XXX, ADR_NONE  , 1}},
    {{OP_BVS, ADR_REL   , 2}, {OP_ADC, ADR_INDY  , 5}, {OP_XXX, ADR_NONE  , 1}, {OP_XXX, ADR_NONE  , 1}, {OP_XXX, ADR_NONE  , 1}, {OP_ADC, ADR_ZPX   , 4}, {OP_ROR, ADR_ZPX   , 6}, {OP_XXX, ADR_NONE  , 1}, {OP_SEI, ADR_IMPL  , 2}, {OP_ADC, ADR_ABSY  , 4}, {OP_XXX, ADR_NONE  , 1}, {OP_XXX, ADR_NONE  , 1}, {OP_XXX, ADR_NONE  , 1}, {OP_ADC, ADR_ABSX  , 4}, {OP_ROR, ADR_ABSX  , 7}, {OP_XXX, ADR_NONE  , 1}},
    {{OP_XXX, ADR_NONE  , 1}, {OP_STA, ADR_INDX  , 6}, {OP_XXX, ADR_NONE  , 1}, {OP_XXX, ADR_NONE  , 1}, {OP_STY, ADR_ZP    , 3}, {OP_STA, ADR_ZP    , 3}, {OP_STX, ADR_ZP    , 3}, {OP_XXX, ADR_NONE  , 1}, {OP_DEY, ADR_IMPL  , 2}, {OP_XXX, ADR_NONE  , 1}, {OP_TXA, ADR_IMPL  , 2}, {OP_XXX, ADR_NONE  , 1}, {OP_STY, ADR_ABS   , 4}, {OP_STA, ADR_ABS   , 4}, {OP_STX, ADR_ABS   , 4}, {OP_XXX, ADR_NONE  , 1}},
    {{OP_BCC, ADR_REL   , 2}, {OP_STA, ADR_INDY  , 6}, {OP_XXX, ADR_NONE  , 1}, {OP_XXX, ADR_NONE  , 1}, {OP_STY, ADR_ZPX   , 4}, {OP_STA, ADR_ZPX   , 4}, {OP_STX, ADR_ZPX   , 4}, {OP_XXX, ADR_NONE  , 1}, {OP_TYA, ADR_IMPL  , 2}, {OP_STA, ADR_ABSY  , 5}, {OP_TXS, ADR_IMPL  , 2}, {OP_XXX, ADR_NONE  , 1}, {OP_XXX, ADR_NONE  , 1}, {OP_STA, ADR_ABSX  , 4}, {OP_XXX, ADR_NONE  , 1}, {OP_XXX, ADR_NONE  , 1}},
    {{OP_LDY, ADR_IMM   , 2}, {OP_LDA, ADR_INDX  , 6}, {OP_LDX, ADR_IMM   , 2}, {OP_XXX, ADR_NONE  , 1}, {OP_LDY, ADR_ZP    , 3}, {OP_LDA, ADR_ZP    , 3}, {OP_LDX, ADR_ZP    , 3}, {OP_XXX, ADR_NONE  , 1}, {OP_TAY, ADR_IMPL  , 2}, {OP_LDA, ADR_IMM   , 2}, {OP_TAX, ADR_IMPL  , 2}, {OP_XXX, ADR_NONE  , 1}, {OP_LDY, ADR_ABS   , 4}, {OP_LDA, ADR_ABS   , 4}, {OP_LDX, ADR_ABS   , 4}, {OP_XXX, ADR_NONE  , 1}},
    {{OP_BCS, ADR_REL   , 2}, {OP_LDA, ADR_INDY  , 5}, {OP_XXX, ADR_NONE  , 1}, {OP_XXX, ADR_NONE  , 1}, {OP_LDY, ADR_ZPX   , 3}, {OP_LDA, ADR_ZPX   , 4}, {OP_LDX, ADR_ZPX   , 4}, {OP_XXX, ADR_NONE  , 1}, {OP_CLV, ADR_IMPL  , 2}, {OP_LDA, ADR_ABSY  , 4}, {OP_TSX, ADR_IMPL  , 2}, {OP_XXX, ADR_NONE  , 1}, {OP_LDY, ADR_ABSX  , 4}, {OP_LDA, ADR_ABSX  , 4}, {OP_ADC, ADR_ABSY  , 4}, {OP_XXX, ADR_NONE  , 1}},
    {{OP_CPY, ADR_IMM   , 2}, {OP_CMP, ADR_INDX  , 6}, {OP_XXX, ADR_NONE  , 1}, {OP_XXX, ADR_NONE  , 1}, {OP_CPY, ADR_ZP    , 3}, {OP_CMP, ADR_ZP    , 3}, {OP_DEC, ADR_ZP    , 5}, {OP_XXX, ADR_NONE  , 1}, {OP_INY, ADR_IMPL  , 2}, {OP_CMP, ADR_IMM   , 2}, {OP_DEX, ADR_IMPL  , 2}, {OP_XXX, ADR_NONE  , 1}, {OP_CPY, ADR_ABS   , 4}, {OP_CMP, ADR_ABS   , 4}, {OP_DEC, ADR_ABS   , 6}, {OP_XXX, ADR_NONE  , 1}},
    {{OP_BNE, ADR_REL   , 2}, {OP_CMP, ADR_INDY  , 5}, {OP_XXX, ADR_NONE  , 1}, {OP_XXX, ADR_NONE  , 1}, {OP_XXX, ADR_NONE  , 1}, {OP_CMP, ADR_ZPX   , 4}, {OP_DEC, ADR_ZPX   , 6}, {OP_XXX, ADR_NONE  , 1}, {OP_CLD, ADR_IMPL  , 2}, {OP_CMP, ADR_ABSY  , 4}, {OP_XXX, ADR_NONE  , 1}, {OP_XXX, ADR_NONE  , 1}, {OP_XXX, ADR_NONE  , 1}, {OP_CMP, ADR_ABSX  , 4}, {OP_DEC, ADR_ABSX  , 7}, {OP_XXX, ADR_NONE  , 1}},
    {{OP_CPX, ADR_IMM   , 2}, {OP_SBC, ADR_INDX  , 6}, {OP_XXX, ADR_NONE  , 1}, {OP_XXX, ADR_NONE  , 1}, {OP_CPX, ADR_ZP    , 3}, {OP_SBC, ADR_ZP    , 3}, {OP_INC, ADR_ZP    , 5}, {OP_XXX, ADR_NONE  , 1}, {OP_INX, ADR_IMPL  , 2}, {OP_SBC, ADR_IMM   , 2}, {OP_NOP, ADR_IMPL  , 2}, {OP_XXX, ADR_NONE  , 1}, {OP_CPX, ADR_ABS   , 4}, {OP_SBC, ADR_ABS   , 4}, {OP_INC, ADR_ABS   , 6}, {OP_XXX, ADR_NONE  , 1}},
    {{OP_BEQ, ADR_REL   , 2}, {OP_SBC, ADR_INDY  , 5}, {OP_XXX, ADR_NONE  , 1}, {OP_XXX, ADR_NONE  , 1}, {OP_XXX, ADR_NONE  , 1}, {OP_SBC, ADR_ZPX   , 4}, {OP_INC, ADR_ZPX   , 6}, {OP_XXX, ADR_NONE  , 1}, {OP_SED, ADR_IMPL  , 2}, {OP_SBC, ADR_ABSY  , 4}, {OP_XXX, ADR_NONE  , 1}, {OP_XXX, ADR_NONE  , 1}, {OP_XXX, ADR_NONE  , 1}, {OP_SBC, ADR_ABSX  , 4}, {OP_INC, ADR_ABSX  , 7}, {OP_XXX, ADR_NONE  , 1}}
};

unsigned short pc;
byte acc, x, y, stkptr, status;
byte memory[0xFFFF];

Instr cur_instr;

byte set_address_mode(byte mode) {
    return 0; // num extra cycles
}

byte execute_instr(byte instr) {
    switch (instr) {
    case OP_ADC: {
        return 0;
    }

    case OP_AND: {
        return 0;
    }

    case OP_ASL: {
        return 0;
    }

    case OP_BCC: {
        return 0;
    }
    
    case OP_BCS: {
        return 0;
    }

    case OP_BEQ: {
        return 0;
    }

    case OP_BIT: {
        return 0;
    }

    case OP_BMI: {
        return 0;
    }

    case OP_BNE: {
        return 0;
    }

    case OP_BPL: {
        return 0;
    }

    case OP_BRK: {
        return 0;
    }

    case OP_BVC: {
        return 0;
    }

    case OP_BVS: {
        return 0;
    }

    case OP_CLC: {
        return 0;
    }

    case OP_CLD: {
        return 0;
    }

    case OP_CLI: {
        return 0;
    }

    case OP_CLV: {
        return 0;
    }

    case OP_CMP: {
        return 0;
    }

    case OP_CPX: {
        return 0;
    }

    case OP_CPY: {
        return 0;
    }

    case OP_DEC: {
        return 0;
    }

    case OP_DEX: {
        return 0;
    }

    case OP_DEY: {
        return 0;
    }

    case OP_EOR: {
        return 0;
    }

    case OP_INC: {
        return 0;
    }

    case OP_INX: {
        return 0;
    }

    case OP_INY: {
        return 0;
    }

    case OP_JMP: {
        return 0;
    }

    case OP_JSR: {
        return 0;
    }

    case OP_LDA: {
        return 0;
    }

    case OP_LDX: {
        return 0;
    }

    case OP_LDY: {
        return 0;
    }

    case OP_LSR: {
        return 0;
    }

    case OP_NOP: {
        return 0;
    }

    case OP_ORA: {
        return 0;
    }

    case OP_PHA: {
        return 0;
    }

    case OP_PHP: {
        return 0;
    }

    case OP_PLA: {
        return 0;
    }

    case OP_PLP: {
        return 0;
    }

    case OP_ROL: {
        return 0;
    }

    case OP_ROR: {
        return 0;
    }

    case OP_RTI: {
        return 0;
    }

    case OP_RTS: {
        return 0;
    }

    case OP_SBC: {
        return 0;
    }

    case OP_SEC: {
        return 0;
    }

    case OP_SED: {
        return 0;
    }

    case OP_SEI: {
        return 0;
    }

    case OP_STA: {
        return 0;
    }

    case OP_STX: {
        return 0;
    }

    case OP_STY: {
        return 0;
    }

    case OP_TAX: {
        x = acc;
        pc++;
        return 0;
    }

    case OP_TAY: {
        y = acc;
        pc++;
        return 0;
    }

    case OP_TSX: {
        return 0;
    }

    case OP_TXA: {
        return 0;
    }

    case OP_TXS: {
        return 0;
    }

    case OP_TYA: {
        return 0;
    }

    return 0;
}

void disp_cpu() {
    printf("acc:[%02X]  x:[%02X]  y:[%02X]  stkptr:[%02X]  pc:[%04X]  status:[N:%c V:%c - B:%c D:%c I:%c Z:%c C:%c]\r",
        acc, x, y, stkptr, pc,
        status & NEGATIVE ? '1' : '0',
        status & OVERFLOW ? '1' : '0',
        status & BRKCOMMAND ? '1' : '0',
        status & DECIMALMODE ? '1' : '0',
        status & IRQDISABLE ? '1' : '0',
        status & ZERO ? '1' : '0',
        status & CARRY ? '1' : '0'
    );
}

byte reset_cpu() {
    acc = 0;
    x = 0;
    y = 0;
}

int main(int argc, char **argv) {
    pc = 0;
    stkptr = 0;
    status = UNUSED;
    cur_instr = {0, 0, 0};

    reset_cpu();

    for (int i = 0; i < 100000; i++) {
        acc++;
        disp_cpu();
    }

    return 0;
}
