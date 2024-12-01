#pragma once

#include <string>
#include <unordered_map>

#include "X86Enums.h"

std::string OpCodeToString(OpCode opCode) {
    switch (opCode) {
    case OpCode::MOV: {
        return "mov";
    }
    }
}

std::string RegCodeToString(RegCode regCode) {
    switch (regCode) {
    case RegCode::AL: {
        return "al";
    }
    case RegCode::CL: {
        return "cl";
    }
    case RegCode::DL: {
        return "dl";
    }
    case RegCode::BL: {
        return "bl";
    }
    case RegCode::AH: {
        return "ah";
    }
    case RegCode::CH: {
        return "ch";
    }
    case RegCode::BH: {
        return "dh";
    }
    case RegCode::DH: {
        return "bh";
    }
    }
}

std::string RegCodeWideToString(RegCodeWide regCodeWide) {
    switch (regCodeWide) {
    case RegCodeWide::AX: {
        return "ax";
    }
    case RegCodeWide::CX: {
        return "cx";
    }
    case RegCodeWide::DX: {
        return "dx";
    }
    case RegCodeWide::BX: {
        return "bx";
    }
    case RegCodeWide::SP: {
        return "sp";
    }
    case RegCodeWide::BP: {
        return "bp";
    }
    case RegCodeWide::SI: {
        return "si";
    }
    case RegCodeWide::DI: {
        return "di";
    }
    }
}