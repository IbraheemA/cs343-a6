#include "student.h"

#include "vendingmachine.h"
#include "bottlingplant.h"

#include <uPRNG.h>
#include <iostream>
#include <fstream>

void Student::main() {
    unsigned int numPurchases = prng(1, maxPurchases);
    BottlingPlant::Flavours favFlavour = (BottlingPlant::Flavours)prng(4); // XXX is cast necessary?
    printer.print(Printer::Kind::Student, id, 'S', favFlavour, numPurchases);

    WATCard::FWATCard watCard = cardOffice.create(id, 5);
    WATCard::FWATCard giftCard = groupoff.giftCard();
    VendingMachine * machine = nameServer.getMachine(id);
    printer.print(Printer::Kind::Student, id, 'V', machine->getId());

    std::ofstream test_out{"t.out", std::ios::app};

    bool giftCardUsed = false;

    while (numPurchases > 0) {
        // Wait before each attempt to buy soda
        yield(prng(1,10));
        for (;;) { // This loop exists to allow retrying without the random wait, for lost cards
            try {
                _Select ( giftCard ) {
                    WATCard & realCard = *giftCard;
                    unsigned int balance = realCard.getBalance();
                    machine->buy(favFlavour, realCard);
                    printer.print(Printer::Kind::Student, id, 'G', favFlavour, balance);
                    delete giftCard;
                    giftCard.reset();
                    giftCardUsed = true;
                } or _Select ( watCard ) {
                    WATCard & realCard = *watCard;
                    unsigned int balance = realCard.getBalance();
                    machine->buy(favFlavour, realCard);
                    printer.print(Printer::Kind::Student, id, 'B', favFlavour, balance);
                }
                numPurchases -= 1; // Successfully bought and paid for a soda; standard outcome
            } catch (VendingMachine::Free &) {
                // Got a free soda; fixed yield then re-attempt 
                _Select ( giftCard ) { // XXX
                    WATCard & realCard = *giftCard;
                    printer.print(Printer::Kind::Student, id, 'a', favFlavour, realCard.getBalance());
                } or _Select ( watCard ) {
                    WATCard & realCard = *watCard;
                    printer.print(Printer::Kind::Student, id, 'A', favFlavour, realCard.getBalance());
                }
                yield(4);
            } catch (VendingMachine::Funds &) {
                _Select ( watCard );
                cardOffice.transfer(id, machine->cost() + 5, watCard);
            } catch (VendingMachine::Stock &) {
                machine = nameServer.getMachine(id);
                printer.print(Printer::Kind::Student, id, 'V', machine->getId());
            } catch(WATCardOffice::Lost &) {
                // If the courier has lost the card, "_Select" will throw "Lost"; get a new card
                printer.print(Printer::Kind::Student, id, 'L');
                watCard = cardOffice.create(id, 5);
                continue; // Loop back in order to retry-without-random-wait
            }
            break; // In all cases except "Lost", exit "retry-without-random-wait" loop
        }
    }

    if (!giftCardUsed) {
        _Select ( giftCard );
        delete giftCard;
    }
    try {
        _Select( watCard );
        delete watCard; // XXX Maybe have to _Select( watCard ) before this bc of the "only buys 1 on gift card case"?
    } catch (WATCardOffice::Lost &) {}

    printer.print(Printer::Kind::Student, id, 'F');
}

Student::Student( Printer & prt, NameServer & nameServer, WATCardOffice & cardOffice, Groupoff & groupoff,
            unsigned int id, unsigned int maxPurchases ) : printer(prt), nameServer(nameServer),
            cardOffice(cardOffice), groupoff(groupoff), id(id), maxPurchases(maxPurchases) {}
