#include <uPRNG.h>

#include "bottlingplant.h"

_Task VendingMachine {
	void main() {
        for (;;) {
            _Accept(inventory) {
                _Accept(restocked);
            } or _Accept(buy);
        }
    }
    Printer & prt;
    unsigned int id;
    unsigned int sodaCost;

    int * inventory;
  public:
	_Event Free {};						// free, advertisement
	_Event Funds {};					// insufficient funds
	_Event Stock {};					// flavour out of stock
	VendingMachine( Printer & prt, NameServer & nameServer, unsigned int id, unsigned int sodaCost ) :
    prt{prt}, id{id}, sodaCost{sodaCost}
    {
        nameServer.VMregister(this);
        inventory = new int[NUM_OF_FLAVOURS];
    }
	void buy( BottlingPlant::Flavours flavour, WATCard & card ) {
        if (card.getBalance < sodaCost) {
            _Throw Funds();
        }
        if (inventory[flavour] <= 0) {
            _Throw Stock();
        }
        inventory[flavour]--;
        if (prng(1,5) == 1) { // 1 in 5 chance the soda is free
            _Throw Free();
        }
        card.withdraw(sodaCost);
    }
	unsigned int * inventory() {
        return inventory;
    }
	void restocked() {}
	_Nomutex unsigned int cost() const {
        return sodaCost;
    }
	_Nomutex unsigned int getId() const {
        return id;
    }
};
