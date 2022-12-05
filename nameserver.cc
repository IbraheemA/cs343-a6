#pragma once

#include "printer.h"
#include "vendingmachine.h"

_Task NameServer {
    Printer & prt;
    unsigned int numVendingMachines;
    unsigned int numStudents;

    int vendingMachineCount;

    int * offsets;

    VendingMachine ** vendingmachines;
    
	void main() {
        while (vendingMachineCount < numVendingMachines) {
            _Accept(VMregister);
        }
    }
  public:
	NameServer( Printer & prt, unsigned int numVendingMachines, unsigned int numStudents ):
    prt{prt}, numVendingMachines{numVendingMachines}, numStudents{numStudents} {
        vendingmachines = new VendinMachine* [numVendingMachines];
        offset = new int[numStudents];

        vendingMachineCount = 0;
    }
	void VMregister( VendingMachine * vendingmachine ) {
        vendingmachines[vendingMachineCount++] = vendingmachine;
    }
	VendingMachine * getMachine( unsigned int id ) {
        return vendingmachines[id + offset[id]++ % numVendingMachines];
    }
	VendingMachine ** getMachineList() {
        return vendingmachines;
    }
};
