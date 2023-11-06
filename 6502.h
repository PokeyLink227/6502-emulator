enum status_flags {
    CARRY       = 0x01,
    ZERO        = 0x02,
    IRQDISABLE  = 0x04,
    DECIMALMODE = 0x08,
    BRKCOMMAND  = 0x10,
    UNUSED      = 0x20,
    OVERFLOW    = 0x40,
    NEGATIVE    = 0x80
};

enum addr_mode {
    ADR_XXX,
    ADR_ACC,
    ADR_IMM,
    ADR_ZPG,
    ADR_ZPX,
    ADR_ZPY,
    ADR_ABS,
    ADR_ABX,
    ADR_ABY,
    ADR_IMP,
    ADR_REL,
    ADR_INX,
    ADR_INY,
    ADR_IND
};

enum opcode {
    OP_XXX,
    OP_ADC,
    OP_AND,
    OP_ASL,
    OP_BCC,
    OP_BCS,
    OP_BEQ,
    OP_BIT,
    OP_BMI,
    OP_BNE,
    OP_BPL,
    OP_BRK,
    OP_BVC,
    OP_BVS,
    OP_CLC,
    OP_CLD,
    OP_CLI,
    OP_CLV,
    OP_CMP,
    OP_CPX,
    OP_CPY,
    OP_DEC,
    OP_DEX,
    OP_DEY,
    OP_EOR,
    OP_INC,
    OP_INX,
    OP_INY,
    OP_JMP,
    OP_JSR,
    OP_LDA,
    OP_LDX,
    OP_LDY,
    OP_LSR,
    OP_NOP,
    OP_ORA,
    OP_PHA,
    OP_PHP,
    OP_PLA,
    OP_PLP,
    OP_ROL,
    OP_ROR,
    OP_RTI,
    OP_RTS,
    OP_SBC,
    OP_SEC,
    OP_SED,
    OP_SEI,
    OP_STA,
    OP_STX,
    OP_STY,
    OP_TAX,
    OP_TAY,
    OP_TSX,
    OP_TXA,
    OP_TXS,
    OP_TYA
};

const char mnemonics[57][4] = {
    "XXX",
    "ADC",
    "AND",
    "ASL",
    "BCC",
    "BCS",
    "BEQ",
    "BIT",
    "BMI",
    "BNE",
    "BPL",
    "BRK",
    "BVC",
    "BVS",
    "CLC",
    "CLD",
    "CLI",
    "CLV",
    "CMP",
    "CPX",
    "CPY",
    "DEC",
    "DEX",
    "DEY",
    "EOR",
    "INC",
    "INX",
    "INY",
    "JMP",
    "JSR",
    "LDA",
    "LDX",
    "LDY",
    "LSR",
    "NOP",
    "ORA",
    "PHA",
    "PHP",
    "PLA",
    "PLP",
    "ROL",
    "ROR",
    "RTI",
    "RTS",
    "SBC",
    "SEC",
    "SED",
    "SEI",
    "STA",
    "STX",
    "STY",
    "TAX",
    "TAY",
    "TSX",
    "TXA",
    "TXS",
    "TYA"
};

typedef unsigned char byte;
typedef unsigned short word;

typedef struct instruction {
    byte opcode, addr_mode, cycles;
} Instr;

struct INES_HEADER {
    byte ID[4];
    byte PRG_ROM_SZ, CHR_ROM_SZ;
    byte FLAGS[5];
    byte PADDING[5];
};

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

void set_flag(byte, unsigned short);
byte get_flag(byte);

byte read(unsigned short);
byte write(unsigned short, byte);

byte set_address_mode(byte);
byte execute_instr(byte);

void reset_cpu();

byte load_rom(const char *, unsigned short);
