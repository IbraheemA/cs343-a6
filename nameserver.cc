#include "nameserver.h"
    
void NameServer::main() {
    while (vendingMachineCount < numVendingMachines) {
        _Accept(VMregister);
    }
}

NameServer::NameServer( Printer & prt, unsigned int numVendingMachines, unsigned int numStudents ):
    prt{prt}, numVendingMachines{numVendingMachines}, numStudents{numStudents} {
    vendingmachines = new VendingMachine* [numVendingMachines];
    offsets = new int[numStudents];

    vendingMachineCount = 0;
}

void NameServer::VMregister( VendingMachine * vendingmachine ) {
    vendingmachines[vendingMachineCount++] = vendingmachine;
}

VendingMachine * NameServer::getMachine( unsigned int id ) {
    return vendingmachines[id + offsets[id]++ % numVendingMachines];
}

VendingMachine ** NameServer::getMachineList() {
    return vendingmachines;
}
