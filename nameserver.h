#pragma once

// #include "printer.h"
// #include "vendingmachine.h"


_Monitor Printer;
_Task VendingMachine;

_Task NameServer {
    Printer & printer;
    unsigned int numVendingMachines;
    unsigned int numStudents;

    int vendingMachineCount;

    int * offsets;

    VendingMachine ** vendingMachines;
    void main();
  public:
    NameServer( Printer & prt, unsigned int numVendingMachines, unsigned int numStudents );
    ~NameServer();
    void VMregister( VendingMachine * vendingmachine );
    VendingMachine * getMachine( unsigned int id ) __attribute__(( warn_unused_result ));
    VendingMachine ** getMachineList() __attribute__(( warn_unused_result ));
};
