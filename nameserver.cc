#include "nameserver.h"

#include <iostream>
#include <fstream>

#include "printer.h"
#include "vendingmachine.h"
    
void NameServer::main() {
    printer.print(Printer::Kind::NameServer, 'S');
    while (vendingMachineCount < numVendingMachines) {
        _Accept(VMregister);
    }

    for (;;) {
        _Accept(~NameServer) {
            break;
        } or _Accept( getMachine || getMachineList );
    }

    std::ofstream test_out{"t.out", std::ios::app};
    printer.print(Printer::Kind::NameServer, 'F');
}

NameServer::NameServer( Printer & prt, unsigned int numVendingMachines, unsigned int numStudents ):
    printer{prt}, numVendingMachines{numVendingMachines}, numStudents{numStudents} {
    vendingMachines = new VendingMachine* [numVendingMachines];
    offsets = new int[numStudents] {0};

    vendingMachineCount = 0;
}

NameServer::~NameServer() {
    delete [] vendingMachines;
    delete [] offsets;
}

void NameServer::VMregister( VendingMachine * vendingmachine ) {
    printer.print(Printer::Kind::NameServer, 'R', vendingmachine->getId());
    vendingMachines[vendingMachineCount++] = vendingmachine;
}

VendingMachine * NameServer::getMachine( unsigned int id ) {
    int index = (id + offsets[id]) % numVendingMachines;
    offsets[id]++;
    VendingMachine * m = vendingMachines[index];
    printer.print(Printer::Kind::NameServer, 'N', id, m->getId());
    return m;
}

VendingMachine ** NameServer::getMachineList() {
    return vendingMachines;
}
