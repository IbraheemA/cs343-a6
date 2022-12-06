#include "groupoff.h"
#include <iostream>
#include <fstream>

Groupoff::Groupoff( Printer & prt, unsigned int numStudents, unsigned int sodaCost,
    unsigned int groupoffDelay ) : printer(prt), numStudents(numStudents), sodaCost(sodaCost),
    groupoffDelay(groupoffDelay), fWATCardPool(numStudents) {}

WATCard::FWATCard Groupoff::giftCard() {
    WATCard::FWATCard ret = fWATCardPool[nextCardToAssign];
    nextCardToAssign += 1;
    return ret;
}

void Groupoff::main() {
    printer.print(Printer::Kind::Groupoff, 'S');
    // First, wait for numStudents calls to giftCard; assume each student only calls it once
    while (nextCardToAssign < numStudents) {
        _Accept(giftCard);
    }

    // Initialize sequence for use in randomization
    int randomSeq[numStudents];
    for (int i = 0; i < numStudents; i += 1) {
        randomSeq[i] = i;
    }
    
    int seqSpaceInUse = numStudents;

    // Once all future giftCards are handed out, start randomly populating them
    for (int i = 0; i < numStudents; i += 1) {
        _Accept(~Groupoff) {
            break;
        } _Else {
            yield(groupoffDelay);
            // Pick a random number from the "in use" portion of the random sequence
            //		(all remaining unused numbers in the interval)
            int randIndex = prng(seqSpaceInUse);
            int num = randomSeq[randIndex];

            // "Remove" used number by "swapping" it with the last number in the seq space then truncating the seq space
            randomSeq[randIndex] = randomSeq[seqSpaceInUse - 1];
            seqSpaceInUse -= 1;

            // Now, populate the randomly selected future card with a real card with balance SodaCost
            WATCard * realCard = new WATCard();
            realCard->deposit(sodaCost);
            fWATCardPool[num].delivery(realCard);
            printer.print(Printer::Kind::Groupoff, 'D', sodaCost);
        }
    }
    printer.print(Printer::Kind::Groupoff, 'F');
}

Groupoff::~Groupoff() {}
