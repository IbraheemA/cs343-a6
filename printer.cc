#include "printer.h"

class Printer::PrintEntry {
    char state;
    unsigned int value1;
    unsigned int value2;
    unsigned int numArgs;

    public:
        PrintEntry( char state) : state{state}, numArgs{1} {}
        PrintEntry( char state, unsigned int value1 ) : state{state}, value1{value1}, numArgs{2} {}
        PrintEntry( char state, unsigned int value1, unsigned int value2 ) : state{state}, value1{value1}, value2{value2}, numArgs{3} {}

        void print() {
            std::cout << state;
            if (numArgs > 1) {
                std::cout << value1;
            }
            if (numArgs > 2) {
                std::cout << "," << value2;
            }
        }
};

void Printer::flushBuffer(int index) {
    if (printBuffer[index] != nullptr) {
        for (int i = 0; i < 6 + numStudents + numVendingMachines + numCouriers; i++) {
            if (printBuffer[i] != nullptr) {
                printBuffer[i]->print();
                delete printBuffer[i];
                printBuffer[i] = nullptr;
            }
            std::cout << "\t";
        }
        std::cout << std::endl;
    }
}

int Printer::getIndex(int kind, unsigned int lid) {
    int offset = 6 + lid;
    switch (kind)
    {
    case 6:
        break;
    case 7:
        offset += numStudents;
        break;
    case 8:
        offset += numStudents + numVendingMachines;
        break;
    default:
        break;
    }
    return offset;
}
Printer::Printer( unsigned int numStudents, unsigned int numVendingMachines, unsigned int numCouriers ) :
    numStudents{numStudents}, numVendingMachines{numVendingMachines}, numCouriers{numCouriers} {
        printBuffer = new PrintEntry*[6 + numStudents + numVendingMachines + numCouriers];

        for (int i = 0; i < 6 + numStudents + numVendingMachines + numCouriers; i++) {
            printBuffer[i] = nullptr;
        }

        std::cout << "Parent\tGropoff\tWATOff\tNames\tTruck\tPlant\t";

        for (int i = 0; i < numStudents; i++) {
            std::cout << "Stud" << i << "\t";
        }
        for (int i = 0; i < numVendingMachines; i++) {
            std::cout << "Mach" << i << "\t";
        }
        for (int i = 0; i < numCouriers; i++) {
            std::cout << "Cour" << i << "\t";
        }
        std::cout << std::endl;
        for (int i = 0; i < 6 + numStudents + numVendingMachines + numCouriers; i++) {
            std::cout << "*******\t";
        }
        std::cout << std::endl;
    }
    
Printer::~Printer() {
    for (int i = 0; i < 6 + numStudents + numVendingMachines + numCouriers; i++) {
        if (printBuffer[i] != nullptr) {
            flushBuffer(i);
            break;
        }
    }

    delete [] printBuffer;

    std::cout << "***********************" << std::endl;
}

void Printer::print( Kind kind, char state ) {
    flushBuffer(kind);
    printBuffer[kind] = new PrintEntry(state);
}
void Printer::print( Kind kind, char state, unsigned int value1 ) {
    flushBuffer(kind);
    printBuffer[kind] = new PrintEntry(state, value1);
}
void Printer::print( Kind kind, char state, unsigned int value1, unsigned int value2 ) {
    flushBuffer(kind);
    printBuffer[kind] = new PrintEntry(state, value1, value2);
}
void Printer::print( Kind kind, unsigned int lid, char state ) {
    int index = getIndex(kind, lid);
    flushBuffer(index);
    printBuffer[index] = new PrintEntry(state);
}
void Printer::print( Kind kind, unsigned int lid, char state, unsigned int value1 ) {
    int index = getIndex(kind, lid);
    flushBuffer(index);
    printBuffer[index] = new PrintEntry(state, value1);
}
void Printer::print( Kind kind, unsigned int lid, char state, unsigned int value1, unsigned int value2 ) {
    int index = getIndex(kind, lid);
    flushBuffer(index);
    printBuffer[index] = new PrintEntry(state, value1, value2);
}
