#include "InstructionDecoder.h"

#include <iostream>

using namespace std;

void parseByte1(uint8_t byte1, ParsedInstruction& inst) {
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
        inst.immediateValueBytesCount = inst.useWideRegs ? DATA16_BASE_SIZE : DATA8_BASE_SIZE;
        inst.bytesCount = MOV_IMM_TO_REG_BASE_SIZE + inst.immediateValueBytesCount;
        break;
    }
    case OPCODE_MOV_IMM_TO_RM:
    {
        inst.useWideRegs = byte1 & OPPARAM_BYTE1_W;
        inst.immediateValueBytesCount = inst.useWideRegs ? DATA16_BASE_SIZE : DATA8_BASE_SIZE;
        inst.bytesCount = MOV_IMM_TO_RM_BASE_SIZE + inst.immediateValueBytesCount;
        break;
    }
    case OPCODE_MOV_ACC_TO_MEM:
    {
        // TODO!
        break;
    }
    case OPCODE_MOV_MEM_TO_ACC:
    {
        // TODO!
        break;
    }
    }
}

void parseByte2(uint8_t byte2, ParsedInstruction& inst) {
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

void parseByte3(uint8_t byte3, ParsedInstruction& inst) {
    switch (inst.opCode) {
    case OPCODE_MOV_RM_TO_RM:
    {
        if (inst.displacementBytesCount >= DATA8_BASE_SIZE) {
            inst.displacementValue = byte3;
        }
        break;
    }
    case OPCODE_MOV_IMM_TO_REG:
    {
        inst.immediateValue += (byte3 << SHIFT_1BYTE);
        break;
    }
    case OPCODE_MOV_IMM_TO_RM:
    {
        if (inst.displacementBytesCount >= DATA8_BASE_SIZE) {
            // 1x displacement
            inst.displacementValue = byte3;
        }
        else {
            // 0x displacement
            inst.immediateValue = byte3;
        }
        break;
    }
    }
}

void parseByte4(uint8_t byte4, ParsedInstruction& inst) {
    switch (inst.opCode) {
    case OPCODE_MOV_RM_TO_RM:
    {
        if (inst.displacementBytesCount >= DATA16_BASE_SIZE) {
            inst.displacementValue += byte4 << SHIFT_1BYTE;
        }
        break;
    }
    case OPCODE_MOV_IMM_TO_RM:
    {
        if (inst.displacementBytesCount >= DATA16_BASE_SIZE) {
            // 2x displacement
            inst.displacementValue += byte4 << SHIFT_1BYTE;
        }
        else if (inst.displacementBytesCount >= DATA8_BASE_SIZE) {
            // 1x displacement
            // 1x immediate
            inst.immediateValue = byte4;
        }
        else {
            // 0x displacement
            if (inst.immediateValueBytesCount >= DATA16_BASE_SIZE) {
                // 2x immediate
                inst.immediateValue += byte4 << SHIFT_1BYTE;
            }
        }
        break;
    }
    }
}

void parseByte5(uint8_t byte5, ParsedInstruction& inst) {
    switch (inst.opCode) {
    case OPCODE_MOV_IMM_TO_RM:
    {
        if (inst.displacementBytesCount >= DATA16_BASE_SIZE) {
            // 2x displacement
            if (inst.immediateValueBytesCount >= DATA8_BASE_SIZE) {
                // 1x immediate
                inst.immediateValue = byte5;
            }
        }
        else if (inst.displacementBytesCount >= DATA8_BASE_SIZE) {
            // 1x displacement
            if (inst.immediateValueBytesCount >= DATA16_BASE_SIZE) {
                // 2x immediate
                inst.immediateValue += byte5 << SHIFT_1BYTE;
            }
        }
        break;
    }
    }
}

void parseByte6(uint8_t byte6, ParsedInstruction& inst) {
    switch (inst.opCode) {
    case OPCODE_MOV_IMM_TO_RM:
    {
        if (inst.displacementBytesCount >= DATA16_BASE_SIZE) {
            if (inst.immediateValueBytesCount >= DATA16_BASE_SIZE) {
                // 2x displacement, 2x immediate
                inst.immediateValue += byte6 << SHIFT_1BYTE;
            }
        }
        break;
    }
    }
}

void testParseByte1() {
    cout << "Testing parseByte1..." << endl;

    {
        cout << "parseByte1(OPCODE_MOV_IMM_TO_REG + OPPARAM_BYTE1_W2 + REGISTER_CX, inst)" << endl;
        ParsedInstruction inst = { 0 };
        parseByte1(OPCODE_MOV_IMM_TO_REG + OPPARAM_BYTE1_W2 + REGISTER_CX, inst);
        if (inst.opCode != OPCODE_MOV_IMM_TO_REG) {
            cout << "\tInvalid opcode: " << int(inst.opCode) << ")" << endl;
        }
        if (inst.bytesCount != 3) {
            cout << "\tInvalid length: " << int(inst.bytesCount) << endl;
        }
        if (!inst.useWideRegs) {
            cout << "\tInvalid w: " << int(inst.useWideRegs) << endl;
        }
        if (inst.reg != REGISTER_CX) {
            cout << "\tInvalid register: " << getRegisterString(inst.reg, inst.useWideRegs) << endl;
        }
        if (inst.regShifted != REGISTER_CX) {
            cout << "\tInvalid register: " << getRegisterString(inst.regShifted, inst.useWideRegs) << endl;
        }
    }

    cout << "Testing parseByte1 complete!" << endl;
}

uint8_t getOpCode(uint8_t byte1) {
    // Order matters, more specific bitmasks should be first
    if ((byte1 & OPCODE_MOV_IMM_TO_REG) == OPCODE_MOV_IMM_TO_REG) {
        return OPCODE_MOV_IMM_TO_REG;
    }
    else if ((byte1 & OPCODE_MOV_IMM_TO_RM) == OPCODE_MOV_IMM_TO_RM) {
        return OPCODE_MOV_IMM_TO_RM;
    }
    else if ((byte1 & OPCODE_MOV_ACC_TO_MEM) == OPCODE_MOV_ACC_TO_MEM) {
        return OPCODE_MOV_ACC_TO_MEM;
    }
    else if ((byte1 & OPCODE_MOV_MEM_TO_ACC) == OPCODE_MOV_MEM_TO_ACC) {
        return OPCODE_MOV_MEM_TO_ACC;
    }
    else if ((byte1 & OPCODE_MOV_RM_TO_RM) == OPCODE_MOV_RM_TO_RM) {
        return OPCODE_MOV_RM_TO_RM;
    }
    else {
        return 0;
    }
}

uint8_t getMode(const ParsedInstruction& mov) {
    return mov.mode >> OPPARAM_BYTE2_MOD_OFFSET;
}

bool isDirectAddressingMode(const ParsedInstruction& mov) {
    return getMode(mov) == OPPARAM_MOD_DISPLACEMENT_NONE && mov.regOrMemShifted == RM_REGISTER_BP;
}

uint8_t getDisplacementBytesCount(const ParsedInstruction& mov) {
    if (isDirectAddressingMode(mov)) {
        return DATA16_BASE_SIZE;
    }

    switch (getMode(mov)) {
    case OPPARAM_MOD_DISPLACEMENT_8BIT:
    {
        return DATA8_BASE_SIZE;
    }
    case OPPARAM_MOD_DISPLACEMENT_16BIT:
    {
        return DATA16_BASE_SIZE;
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
    case OPCODE_MOV_MEM_TO_ACC:
    case OPCODE_MOV_ACC_TO_MEM:
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

string get8086IntegerAsString(int value, bool wide) {
    return to_string(wide ? int16_t(value) : int(int8_t(value)));
}

string getInferredSize8086IntegerAsString(int value) {
    if (value > UINT8_MAX) {
        return "word " + to_string(uint16_t(value));
    }
    else {
        return "byte " + to_string(int(uint8_t(value)));
    }
}

string getRmString(const ParsedInstruction& mov) {
    if (isDirectAddressingMode(mov)) {
        return getMemString(mov.displacementValue);
    }

    string result;
    switch (mov.mode >> OPPARAM_BYTE2_MOD_OFFSET) {
    case OPPARAM_MOD_DISPLACEMENT_NONE:
    case OPPARAM_MOD_DISPLACEMENT_8BIT:
    case OPPARAM_MOD_DISPLACEMENT_16BIT:
    {
        result = "[" + getBaseRmValue(mov.regOrMemShifted);
        if (mov.displacementValue != 0) {
            const string intAsString = get8086IntegerAsString(mov.displacementValue, mov.displacementBytesCount >= DATA16_BASE_SIZE);
            if (intAsString[0] == '-') {
                result += " - " + intAsString.substr(1);
            }
            else {
                result += " + " + intAsString;
            }
        }
        result += "]";
        break;
    }
    case OPPARAM_MOD_REGISTER:
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

string getMemString(uint16_t displacement) {
    return "[" + to_string(displacement) + "]";
}

string instructionToString(const ParsedInstruction& inst) {
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
    case OPCODE_MOV_IMM_TO_RM:
    {
        result += getRmString(inst);
        result += ", ";
        result += getInferredSize8086IntegerAsString(inst.immediateValue);
        break;
    }
    }

    return result;
}
