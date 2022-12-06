#include <uPRNG.h>

#include <iostream>
#include <fstream>

#include "bottlingplant.h"
#include "vendingmachine.h"
#include "nameserver.h"

void VendingMachine::main() {
    printer.print(Printer::Kind::Vending, 'S', sodaCost);
    for (;;) {
        try {
            _Accept(~VendingMachine) {
                break;
            } or _Accept(inventory) {
                printer.print(Printer::Kind::Vending, 'r');
                _Accept(restocked);
                printer.print(Printer::Kind::Vending, 'R');
            } or _Accept(buy);
        } catch( uMutexFailure::RendezvousFailure & ) {
            // XXX
        }
    }
    printer.print(Printer::Kind::Vending, 'F');
}

VendingMachine::VendingMachine( Printer & prt, NameServer & nameServer, unsigned int id, unsigned int sodaCost ) :
printer{prt}, id{id}, sodaCost{sodaCost}
{
    nameServer.VMregister(this);
    currentInventory = new unsigned int[NUM_OF_FLAVOURS];
}

void VendingMachine::buy( BottlingPlant::Flavours flavour, WATCard & card ) {
    std::ofstream test_out{"t.out", std::ios::app};
    // std::osacquire(/**/std::cout) << "buy time" << std::endl;
    if (card.getBalance() < sodaCost) {
        // std::osacquire(/**/std::cout) << "u broke" << std::endl;
        _Throw Funds();
    }
    if (currentInventory[flavour] == 0) {
        // std::osacquire(/**/std::cout) << "no stock" << std::endl;
        _Throw Stock();
    }
    currentInventory[flavour]--;
    if (prng(1,5) == 1) { // 1 in 5 chance the soda is free
        printer.print(Printer::Kind::Vending, 'A');
        _Throw Free();
    }
    card.withdraw(sodaCost);
    printer.print(Printer::Kind::Vending, 'B', flavour, currentInventory[flavour]);
    // std::osacquire(/**/std::cout) << "done buying" << std::endl;
}
unsigned int * VendingMachine::inventory() {
    return currentInventory;
}
void VendingMachine::restocked() {}
_Nomutex unsigned int VendingMachine::cost() const {
    return sodaCost;
}
unsigned int VendingMachine::getId() const {
    return id;
}
