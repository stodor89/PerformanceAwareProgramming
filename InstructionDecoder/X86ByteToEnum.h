#include "X86Enums.h"

OpCode byte1ToOpCode(uint8_t byte1) {
    switch (OpCode(byte1 >> FieldStart::OPCODE)) {
    case OpCode::MOV: return OpCode::MOV;
    }
}