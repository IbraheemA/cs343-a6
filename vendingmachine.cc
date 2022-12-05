#include <uPRNG.h>

#include "bottlingplant.h"
#include "vendingmachine.h"
#include "nameserver.h"

void VendingMachine::main() {
    for (;;) {
        _Accept(inventory) {
            _Accept(restocked);
        } or _Accept(buy);
    }
}

VendingMachine::VendingMachine( Printer & prt, NameServer & nameServer, unsigned int id, unsigned int sodaCost ) :
prt{prt}, id{id}, sodaCost{sodaCost}
{
    nameServer.VMregister(this);
    currentInventory = new unsigned int[NUM_OF_FLAVOURS];
}

void VendingMachine::buy( BottlingPlant::Flavours flavour, WATCard & card ) {
    if (card.getBalance() < sodaCost) {
        _Throw Funds();
    }
    if (currentInventory[flavour] == 0) {
        _Throw Stock();
    }
    currentInventory[flavour]--;
    if (prng(1,5) == 1) { // 1 in 5 chance the soda is free
        _Throw Free();
    }
    card.withdraw(sodaCost);
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
