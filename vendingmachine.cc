#include <uPRNG.h>

_Task VendingMachine {
	void main() {
        for (;;) {}
    }
    Printer & prt;
    unsigned int id;
    unsigned int sodaCost;

    int * inventory;
    bool restocking = false; // TODO: maybe rework this to not use a flag
  public:
	_Event Free {};						// free, advertisement
	_Event Funds {};					// insufficient funds
	_Event Stock {};					// flavour out of stock
	VendingMachine( Printer & prt, NameServer & nameServer, unsigned int id, unsigned int sodaCost ) :
    prt{prt}, id{id}, sodaCost{sodaCost}
    {
        nameServer.VMregister(this);
        inventory = new int[4]; // TODO: number of soda flavours - update to a constant or a vector this is ugly
    }
	void buy( BottlingPlant::Flavours flavour, WATCard & card ) {
        if (restocking) {
            _Accept(restocked);
        }
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
        restocking = true;
        return inventory;
    }
	void restocked() {
        restocking = false;
    }
	_Nomutex unsigned int cost() const {
        return sodaCost;
    }
	_Nomutex unsigned int getId() const {
        return id;
    }
};
