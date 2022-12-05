#include "nameserver.h"
    
void NameServer::main() {
    while (vendingMachineCount < numVendingMachines) {
        _Accept(VMregister);
    }
}

NameServer::NameServer( Printer & prt, unsigned int numVendingMachines, unsigned int numStudents ):
    prt{prt}, numVendingMachines{numVendingMachines}, numStudents{numStudents} {
    vendingMachines = new VendingMachine* [numVendingMachines];
    offsets = new int[numStudents];

    vendingMachineCount = 0;
}

void NameServer::VMregister( VendingMachine * vendingmachine ) {
    vendingMachines[vendingMachineCount++] = vendingmachine;
}

VendingMachine * NameServer::getMachine( unsigned int id ) {
    return vendingMachines[id + offsets[id]++ % numVendingMachines];
}

VendingMachine ** NameServer::getMachineList() {
    return vendingMachines;
}
