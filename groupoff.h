#pragma once

#include "watcard.h"

_Task Groupoff {
    void main();

    Printer & printer;
    unsigned int numStudents;
    unsigned int sodaCost;
    unsigned int groupoffDelay;

    std::vector<WATCard::FWATCard> fWATCardPool;

    unsigned int nextCardToAssign = 0;
  public:
    Groupoff( Printer & prt, unsigned int numStudents, unsigned int sodaCost, unsigned int groupoffDelay );
    WATCard::FWATCard giftCard();
};
