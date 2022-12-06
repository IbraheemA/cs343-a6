#include "student.h"

#include "vendingmachine.h"
#include "bottlingplant.h"

#include <uPRNG.h>
#include <iostream>
#include <fstream>

void Student::main() {
    unsigned int numPurchases = prng(1, maxPurchases);
    BottlingPlant::Flavours favFlavour = (BottlingPlant::Flavours)prng(4); // XXX is cast necessary?
    printer.print(Printer::Kind::Student, 'S', favFlavour, numPurchases);

    WATCard::FWATCard watCard = cardOffice.create(id, 5);
    WATCard::FWATCard giftCard = groupoff.giftCard();
    VendingMachine * machine = nameServer.getMachine(id);
    printer.print(Printer::Kind::Student, 'V', machine->getId());

    std::ofstream test_out{"t.out", std::ios::app};
    // std::osacquire(/**/std::cout) << "commence student B)" << id << std::endl;

    while (numPurchases > 0) {
        // std::osacquire(/**/std::cout) << "numPurchases: " << id << numPurchases << std::endl;
        // Wait before each attempt to buy soda
        yield(prng(1,10));
        for (;;) { // This loop exists to allow retrying without the random wait, for lost cards
            // std::osacquire(/**/std::cout) << "dmubassery: " << id << numPurchases << std::endl;
            try {
                // std::osacquire(/**/std::cout) << "select time " << id << std::endl;
                _Select ( giftCard ) {
                    WATCard & realCard = *giftCard;
                    unsigned int balance = realCard.getBalance();
                    machine->buy(favFlavour, realCard);
                    printer.print(Printer::Kind::Student, 'G', favFlavour, balance);
                    giftCard.reset();
                } or _Select ( watCard ) {
                    WATCard & realCard = *watCard;
                    unsigned int balance = realCard.getBalance();
                    machine->buy(favFlavour, realCard);
                    printer.print(Printer::Kind::Student, 'B', favFlavour, balance);
                }
                // std::osacquire(/**/std::cout) << "decrement time" << id << std::endl;
                numPurchases -= 1; // Successfully bought and paid for a soda; standard outcome
            } catch (VendingMachine::Free &) {
                // std::osacquire(/**/std::cout) << "free soda time" << id << std::endl;
                // Got a free soda; fixed yield then re-attempt 
                _Select ( giftCard ) { // XXX
                    WATCard & realCard = *giftCard;
                    printer.print(Printer::Kind::Student, 'a', favFlavour, realCard.getBalance());
                } or _Select ( watCard ) {
                    WATCard & realCard = *watCard;
                    printer.print(Printer::Kind::Student, 'A', favFlavour, realCard.getBalance());
                }
                yield(4);
            } catch (VendingMachine::Funds &) {
                // std::osacquire(/**/std::cout) << "broke funds time" << id << std::endl;
                _Select ( watCard );
                // std::cout << "time to transfer " << id << std::endl;
                cardOffice.transfer(id, machine->cost() + 5, watCard);
            } catch (VendingMachine::Stock &) {
                // std::osacquire(/**/std::cout) << "stonks time" << id << std::endl;
                machine = nameServer.getMachine(id);
                printer.print(Printer::Kind::Student, 'V', machine->getId());
            } catch(WATCardOffice::Lost &) {
                // If the courier has lost the card, "_Select" will throw "Lost"; get a new card
                // std::osacquire(/**/std::cout) << "lost time" << id << std::endl;
                printer.print(Printer::Kind::Student, 'L');
                watCard = cardOffice.create(id, 5);
                continue; // Loop back in order to retry-without-random-wait
            }
            // std::cout << "break time " << id << std::endl;
            break; // In all cases except "Lost", exit "retry-without-random-wait" loop
        }
    }

    // std::osacquire(/**/std::cout) << "suicide time B) " << id << std::endl;

    // std::cout << "time to delete watcard " << id << std::endl;
    // _Select( watCard );
    // delete watCard; // XXX Maybe have to _Select( watCard ) before this bc of the "only buys 1 on gift card case"? 

    // std::osacquire(/**/std::cout) << "successful suicide B) " << id << std::endl;

    printer.print(Printer::Kind::Student, 'F');
}

Student::Student( Printer & prt, NameServer & nameServer, WATCardOffice & cardOffice, Groupoff & groupoff,
            unsigned int id, unsigned int maxPurchases ) : printer(prt), nameServer(nameServer),
            cardOffice(cardOffice), groupoff(groupoff), id(id), maxPurchases(maxPurchases) {}
