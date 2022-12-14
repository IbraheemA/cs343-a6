#pragma once

#include "printer.h"
#include "bottlingplant.h"
#include "watcard.h"

#include "nameserver.h"
// class NameServer;

_Task VendingMachine {
	Printer & printer;
    unsigned int id;
    unsigned int sodaCost;

    unsigned int * currentInventory;
	void main();

	uOwnerLock mlk;
  public:
	_Event Free {};						// free, advertisement
	_Event Funds {};					// insufficient funds
	_Event Stock {};					// flavour out of stock
	VendingMachine( Printer & prt, NameServer & nameServer, unsigned int id, unsigned int sodaCost );
	~VendingMachine();
	void buy( BottlingPlant::Flavours flavour, WATCard & card );
	unsigned int * inventory() __attribute__(( warn_unused_result ));
	void restocked();
	_Nomutex unsigned int cost() const;
	_Nomutex unsigned int getId() const;
};
