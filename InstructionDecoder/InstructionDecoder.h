#pragma once

#include <cstdint>
#include <string>

// Registers
constexpr uint8_t REGISTER_AX = 0b000;
constexpr uint8_t REGISTER_CX = 0b001;
constexpr uint8_t REGISTER_DX = 0b010;
constexpr uint8_t REGISTER_BX = 0b011;
constexpr uint8_t REGISTER_SP = 0b100;
constexpr uint8_t REGISTER_BP = 0b101;
constexpr uint8_t REGISTER_SI = 0b110;
constexpr uint8_t REGISTER_DI = 0b111;
constexpr uint8_t REGISTER_AL = 0b000;
constexpr uint8_t REGISTER_CL = 0b001;
constexpr uint8_t REGISTER_DL = 0b010;
constexpr uint8_t REGISTER_BL = 0b011;
constexpr uint8_t REGISTER_AH = 0b100;
constexpr uint8_t REGISTER_CH = 0b101;
constexpr uint8_t REGISTER_DH = 0b110;
constexpr uint8_t REGISTER_BH = 0b111;

// Registers as used in R/M encoding
constexpr uint8_t RM_REGISTER_BX_SI = 0b000;
constexpr uint8_t RM_REGISTER_BX_DI = 0b001;
constexpr uint8_t RM_REGISTER_BP_SI = 0b010;
constexpr uint8_t RM_REGISTER_BP_DI = 0b011;
constexpr uint8_t RM_REGISTER_SI = 0b100;
constexpr uint8_t RM_REGISTER_DI = 0b101;
constexpr uint8_t RM_REGISTER_BP = 0b110;
constexpr uint8_t RM_REGISTER_BX = 0b111;

// OpCodes
constexpr uint8_t OPCODE_MOV_RM_TO_RM = 0b10001000;
constexpr uint8_t OPCODE_MOV_IMM_TO_RM = 0b11000110;
constexpr uint8_t OPCODE_MOV_IMM_TO_REG = 0b10110000;

// Move modes
constexpr uint8_t MOV_MOD_DISPLACEMENT_NONE = 0b00;
constexpr uint8_t MOV_MOD_DISPLACEMENT_8BIT = 0b01;
constexpr uint8_t MOV_MOD_DISPLACEMENT_16BIT = 0b10;
constexpr uint8_t MOV_MOD_REGISTER = 0b11;

// Sizes
constexpr uint8_t MOV_IMM_TO_REG_BASE_SIZE = 1;
constexpr uint8_t MOV_RM_TO_RM_BASE_SIZE = 2;
constexpr uint8_t DISP8_BASE_SIZE = 1;
constexpr uint8_t DISP16_BASE_SIZE = 2;
constexpr uint8_t SHIFT_1BYTE = 8;

// Operation parameters locations
constexpr uint8_t OPPARAM_BYTE1_W_OFFSET = 0;
constexpr uint8_t OPPARAM_BYTE1_D_OFFSET = 1;
constexpr uint8_t OPPARAM_BYTE1_OPCODE_OFFSET = 2;
constexpr uint8_t OPPARAM_BYTE1_REG_OFFSET = 0;
constexpr uint8_t OPPARAM_BYTE1_W2_OFFSET = 3;
constexpr uint8_t OPPARAM_BYTE2_RM_OFFSET = 0;
constexpr uint8_t OPPARAM_BYTE2_REG_OFFSET = 3;
constexpr uint8_t OPPARAM_BYTE2_MOD_OFFSET = 6;

// Operation parameters bitmasks
constexpr uint8_t OPPARAM_BYTE1_W = 0b1;
constexpr uint8_t OPPARAM_BYTE1_D = 0b10;
constexpr uint8_t OPPARAM_BYTE1_REG = 0b111;
constexpr uint8_t OPPARAM_BYTE1_W2 = 0b1000;
constexpr uint8_t OPPARAM_BYTE2_MOD = 0b11000000;
constexpr uint8_t OPPARAM_BYTE2_REG = 0b111000;
constexpr uint8_t OPPARAM_BYTE2_RM = 0b111;

struct MovInstruction {
    // Regular
    uint8_t opCode;
    uint8_t regIsDestination;
    uint8_t useWideRegs;
    uint8_t mode;
    uint8_t reg;
    uint8_t regOrMem;
    uint8_t immediateValueBytesCount;
    uint16_t immediateValue;
    uint8_t bytesCount;
    uint8_t displacementBytesCount;
    uint16_t displacementValue;

    // Shifted to rightmost bits
    uint8_t regOrMemShifted;
    uint8_t regShifted;
};

// Main parsing functions
void parseByte1(uint8_t byte1, MovInstruction& inst);
void parseByte2(uint8_t byte2, MovInstruction& inst);
void parseByte3(uint8_t byte3, MovInstruction& inst);
void parseByte4(uint8_t byte4, MovInstruction& inst);

// Tests
void testParseByte1();

// Parsing helpers
uint8_t getOpCode(uint8_t byte1);
uint8_t getDisplacementBytesCount(const MovInstruction& mov);

// Stringification helpers
std::string getOpCodeString(uint8_t opCode);
std::string getRegisterString(uint8_t reg, bool wide);
std::string getBaseRmValue(uint8_t rm);
std::string get8086IntegerAsString(int value, bool useWideRegs);
std::string getRmString(MovInstruction mov);
std::string instructionToString(const MovInstruction& inst);
