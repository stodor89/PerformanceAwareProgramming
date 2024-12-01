#pragma once

#include <stdint.h>
#include <unordered_map>

enum FieldLength {
    // byte 1
    OPCODE = 6,
    D = 1,
    W = 1,
    // byte 2
    MOD = 2,
    REG = 3,
    RM = 3,
};

enum FieldStart {
    W = 0,
    D = 1,
    OPCODE = 2,
    RM = 0,
    REG = 3,
    MOD = 6
};

enum class OpCode : uint8_t {
    MOV = 0b100010,
};

enum class RegCode : uint8_t {
    AL = 0b000,
    CL = 0b001,
    DL = 0b010,
    BL = 0b011,
    AH = 0b100,
    CH = 0b101,
    DH = 0b110,
    BH = 0b111,
};

enum RegCodeWide : uint8_t {
    AX = 0b000,
    CX = 0b001,
    DX = 0b010,
    BX = 0b011,
    SP = 0b100,
    BP = 0b101,
    SI = 0b110,
    DI = 0b111,
};
