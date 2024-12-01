#include "InstructionDecoder.h"

#include <fstream>
#include <iostream>
#include <vector>

using namespace std;

string processBinaryFile(const std::string& fileName) {
    vector<uint8_t> bytes;
    {
        ifstream file(fileName, ifstream::binary);
        if (!file) {
            cout << "Could not open " << fileName << endl;
            exit(1);
        }

        file.seekg(0, file.end);
        auto length = file.tellg();
        file.seekg(0, file.beg);
        bytes.resize(length);
        file.read((char*)bytes.data(), length);
    }

    string result;
    int byteNumber = 1;
    MovInstruction inst = { 0 };
    for (size_t i = 0; i < bytes.size(); i++) {
        switch (byteNumber) {
        case 0:
        {
            result += instructionToString(inst) + "\n";
            inst = { 0 };
            byteNumber = 1;
            [[fallthrough]];
        }
        case 1: 
        {
            parseByte1(bytes[i], inst);
            break;
        }
        case 2:
        {
            parseByte2(bytes[i], inst);
            break;
        }
        case 3:
        {
            parseByte3(bytes[i], inst);
            break;
        }
        case 4:
        {
            parseByte4(bytes[i], inst);
            break;
        }
        }

        if (inst.bytesCount == 0) {
            continue;
        }
        byteNumber = (byteNumber + 1) % (inst.bytesCount + 1);
    }
    result += instructionToString(inst) + "\n";

    return result;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        cout << "Usage: InstructionDecoder <filename>" << endl;
        return 1;
    }

    const string argv1 = argv[1];
    if (argv1 == "--test") {
        cout << "Running tests!" << endl;
        testParseByte1();
        cout << "End of tests!" << endl;
        return 0;
    }

    // Use like this to record the result to a file:
    // C:\Users\stodo\source\repos\sim8086\x64\Debug> .\InstructionDecoder.exe ..\..\InstructionDecoder\listing_0038_many_register_mov > listing0038.asm
    // Then build with NASM:
    // nasm -o listing0038 listing0038.asm
    // Then you can compare it to the original file like this:
    // C:\Users\stodo\source\repos\sim8086\x64\Debug>fc listing0038 ..\..\InstructionDecoder\listing_0038_many_register_mov
    // The two files should be the same.
    cout << processBinaryFile(argv1) << endl;
}