#include "InstructionDecoder.h"

#include <iostream>

using namespace std;

void parseByte1(uint8_t byte1, MovInstruction& inst) {
    inst.opCode = getOpCode(byte1);
    switch (inst.opCode) {
    case OPCODE_MOV_RM_TO_RM:
    {
        inst.regIsDestination = byte1 & OPPARAM_BYTE1_D;
        inst.useWideRegs = byte1 & OPPARAM_BYTE1_W;
        inst.bytesCount = MOV_RM_TO_RM_BASE_SIZE;
        break;
    }
    case OPCODE_MOV_IMM_TO_REG:
    {
        inst.reg = byte1 & OPPARAM_BYTE1_REG;
        inst.regShifted = inst.reg;
        inst.useWideRegs = byte1 & OPPARAM_BYTE1_W2;
        inst.regIsDestination = true;
        inst.immediateValueBytesCount = inst.useWideRegs ? DISP16_BASE_SIZE : DISP8_BASE_SIZE;
        inst.bytesCount = MOV_IMM_TO_REG_BASE_SIZE + inst.immediateValueBytesCount;
        break;
    }
    }
}

void parseByte2(uint8_t byte2, MovInstruction& inst) {
    switch (inst.opCode) {
    case OPCODE_MOV_RM_TO_RM:
    {
        inst.mode = (byte2 & OPPARAM_BYTE2_MOD);
        inst.reg = (byte2 & OPPARAM_BYTE2_REG);
        inst.regShifted = inst.reg >> OPPARAM_BYTE2_REG_OFFSET;
        inst.regOrMem = (byte2 & OPPARAM_BYTE2_RM);
        inst.regOrMemShifted = inst.regOrMem >> OPPARAM_BYTE2_RM_OFFSET;
        inst.displacementBytesCount = getDisplacementBytesCount(inst);
        inst.bytesCount += inst.displacementBytesCount;
        break;
    }
    case OPCODE_MOV_IMM_TO_REG:
    {
        inst.immediateValue = byte2;
        break;
    }
    case OPCODE_MOV_IMM_TO_RM:
    {
        inst.mode = (byte2 & OPPARAM_BYTE2_MOD);
        inst.regOrMem = (byte2 & OPPARAM_BYTE2_RM);
        inst.regOrMemShifted = inst.regOrMem >> OPPARAM_BYTE2_RM_OFFSET;
        inst.displacementBytesCount = getDisplacementBytesCount(inst);
        inst.bytesCount += inst.displacementBytesCount;
        break;
    }
    }
}

void parseByte3(uint8_t byte3, MovInstruction& inst) {
    switch (inst.opCode) {
    case OPCODE_MOV_RM_TO_RM:
    {
        if (inst.displacementBytesCount >= DISP8_BASE_SIZE) {
            inst.displacementValue = byte3;
        }
        break;
    }
    case OPCODE_MOV_IMM_TO_REG:
    {
        inst.immediateValue += (byte3 << SHIFT_1BYTE);
        break;
    }
    }
}

void parseByte4(uint8_t byte4, MovInstruction& inst) {
    switch (inst.opCode) {
    case OPCODE_MOV_RM_TO_RM:
    {
        if (inst.displacementBytesCount >= DISP16_BASE_SIZE) {
            inst.displacementValue += byte4 << SHIFT_1BYTE;
        }
        break;
    }
    }
}

void testParseByte1() {
    cout << "Testing parseByte1..." << endl;

    {
        cout << "parseByte1(OPCODE_MOV_IMM_TO_REG + OPPARAM_BYTE1_W2 + REGISTER_CX, mov)" << endl;
        MovInstruction mov = { 0 };
        parseByte1(OPCODE_MOV_IMM_TO_REG + OPPARAM_BYTE1_W2 + REGISTER_CX, mov);
        if (mov.opCode != OPCODE_MOV_IMM_TO_REG) {
            cout << "\tInvalid opcode: " << int(mov.opCode) << ")" << endl;
        }
        if (mov.bytesCount != 3) {
            cout << "\tInvalid length: " << int(mov.bytesCount) << endl;
        }
        if (!mov.useWideRegs) {
            cout << "\tInvalid w: " << int(mov.useWideRegs) << endl;
        }
        if (mov.reg != REGISTER_CX) {
            cout << "\tInvalid register: " << getRegisterString(mov.reg, mov.useWideRegs) << endl;
        }
        if (mov.regShifted != REGISTER_CX) {
            cout << "\tInvalid register: " << getRegisterString(mov.regShifted, mov.useWideRegs) << endl;
        }
    }

    cout << "Testing parseByte1 complete!" << endl;
}

uint8_t getOpCode(uint8_t byte1) {
    // Order matters, more specific bitmasks should be first
    if ((byte1 & OPCODE_MOV_IMM_TO_REG) == OPCODE_MOV_IMM_TO_REG) {
        return OPCODE_MOV_IMM_TO_REG;
    }
    else if ((byte1 & OPCODE_MOV_RM_TO_RM) == OPCODE_MOV_RM_TO_RM) {
        return OPCODE_MOV_RM_TO_RM;
    }
    else {
        return 0;
    }
}

uint8_t getDisplacementBytesCount(const MovInstruction& mov) {
    switch (mov.mode >> OPPARAM_BYTE2_MOD_OFFSET) {
    case MOV_MOD_DISPLACEMENT_NONE:
    {
        // special case: mod = 00 && rm = bp means direct address
        // which is 16 bits on a 8086
        return (mov.regOrMemShifted == RM_REGISTER_BP) ? DISP16_BASE_SIZE : 0;
    }
    case MOV_MOD_DISPLACEMENT_8BIT:
    {
        return DISP8_BASE_SIZE;
    }
    case MOV_MOD_DISPLACEMENT_16BIT:
    {
        return DISP16_BASE_SIZE;
    }
    default:
    {
        return 0;
    }
    }
}

string getOpCodeString(uint8_t opCode) {
    switch (opCode) {
    case OPCODE_MOV_RM_TO_RM:
    case OPCODE_MOV_IMM_TO_REG:
    case OPCODE_MOV_IMM_TO_RM:
        return "mov";
    default: return "undefined_opcode";
    }
}

string getRegisterString(uint8_t reg, bool wide) {
    if (wide) {
        switch (reg) {
        case REGISTER_AX: return "ax";
        case REGISTER_CX: return "cx";
        case REGISTER_DX: return "dx";
        case REGISTER_BX: return "bx";
        case REGISTER_SP: return "sp";
        case REGISTER_BP: return "bp";
        case REGISTER_SI: return "si";
        case REGISTER_DI: return "di";
        default: return "undefined_word_register";
        }
    }
    else {
        switch (reg) {
        case REGISTER_AL: return "al";
        case REGISTER_CL: return "cl";
        case REGISTER_DL: return "dl";
        case REGISTER_BL: return "bl";
        case REGISTER_AH: return "ah";
        case REGISTER_CH: return "ch";
        case REGISTER_DH: return "dh";
        case REGISTER_BH: return "bh";
        default: return "undefined_byte_register";
        }
    }
}

string getBaseRmValue(uint8_t rm) {
    switch (rm) {
    case RM_REGISTER_BX_SI: return "bx + si";
    case RM_REGISTER_BX_DI: return "bx + di";
    case RM_REGISTER_BP_SI: return "bp + si";
    case RM_REGISTER_BP_DI: return "bp + di";
    case RM_REGISTER_SI: return "si";
    case RM_REGISTER_DI: return "di";
    case RM_REGISTER_BP: return "bp";
    case RM_REGISTER_BX: return "bx";
    default: return "undefined_base_rm";
    }
}

string get8086IntegerAsString(int value, bool useWideRegs) {
    return to_string(useWideRegs ? int(int16_t(value)) : int(int8_t(value)));
}

string getRmString(MovInstruction mov) {
    string result;
    switch (mov.mode >> OPPARAM_BYTE2_MOD_OFFSET) {
    case MOV_MOD_DISPLACEMENT_NONE:
    case MOV_MOD_DISPLACEMENT_8BIT:
    case MOV_MOD_DISPLACEMENT_16BIT:
    {
        result = "[" + getBaseRmValue(mov.regOrMemShifted);
        if (mov.displacementValue != 0) {
            result += " + " + get8086IntegerAsString(mov.displacementValue, mov.displacementBytesCount >= DISP16_BASE_SIZE);
        }
        result += "]";
        break;
    }
    case MOV_MOD_REGISTER:
    {
        result = getRegisterString(mov.regOrMemShifted, mov.useWideRegs);
        break;
    }
    default:
    {
        result = "undefined_rm";
    }
    }

    return result;
}

string instructionToString(const MovInstruction& inst) {
    string result = getOpCodeString(inst.opCode);
    result += " ";

    switch (inst.opCode) {
    case OPCODE_MOV_RM_TO_RM:
    {
        if (inst.regIsDestination) {
            result += getRegisterString(inst.regShifted, inst.useWideRegs);
            result += ", ";
            result += getRmString(inst);
        }
        else { // REG is source
            result += getRmString(inst);
            result += ", ";
            result += getRegisterString(inst.regShifted, inst.useWideRegs);
        }
        break;
    }
    case OPCODE_MOV_IMM_TO_REG:
    {
        result += getRegisterString(inst.reg, inst.useWideRegs);
        result += ", ";
        result += get8086IntegerAsString(inst.immediateValue, inst.immediateValueBytesCount > 1);
        break;
    }
    }

    return result;
}
