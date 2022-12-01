#pragma once

#include "student.h"
#include "nameserver.h"
#include "watcardoffice.h"
#include "groupoff.h"

_Task Student {
	void main();

	Printer & printer;
	NameServer & nameServer;
	WATCardOffice & cardOffice;
	Groupoff & groupoff;
	unsigned int id;
	unsigned int maxPurchases;
  public:
	Student( Printer & prt, NameServer & nameServer, WATCardOffice & cardOffice, Groupoff & groupoff,
			 unsigned int id, unsigned int maxPurchases );
};
