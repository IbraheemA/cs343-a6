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
    void main();
  public:
    NameServer( Printer & prt, unsigned int numVendingMachines, unsigned int numStudents );
    void VMregister( VendingMachine * vendingmachine );
    VendingMachine * getMachine( unsigned int id ) __attribute__(( warn_unused_result ));
    VendingMachine ** getMachineList() __attribute__(( warn_unused_result ));
};
