#include "student.h"

#include "vendingmachine.h"
#include "bottlingplant.h"

#include <prng.h>

void Student::main() {
    unsigned int numPurchases = prng(1, maxPurchases);
    BottlingPlant::Flavours favFlavour = (BottlingPlant::Flavours)prng(4); // XXX is cast necessary?

    WATCard::FWATCard watCard = cardOffice.create(id, 5);
    WATCard::FWATCard giftCard = groupoff.giftCard();
    VendingMachine * machine = nameServer.getMachine(id);

    while (numPurchases > 0) {
        // Wait before each attempt to buy soda
        yield(prng(1,10));
        for (;;) { // This loop exists to allow retrying without the random wait, for lost cards
            try {
                _Select ( giftCard ) {
                    machine.buy(favFlavour, *giftCard);
                    giftCard.reset();
                } or _Select ( watCard ) {
                    machine.buy(favFlavour, *watCard);
                }
                numPurchases -= 1; // Successfully bought and paid for a soda; standard outcome
            } catch (VendingMachine::Free &) {
                // Got a free soda; fixed yield then re-attempt 
                yield(4);
            } catch (VendingMachine::Funds &) {
                cardOffice.transfer(id, machine.cost() + 5, watCard);
            } catch (VendingMachine::Stock &) {
                machine = nameServer.getMachine(id);
            } catch(WATCardOffice::Lost &) {
                // If the courier has lost the card, "_Select" will throw "Lost"; get a new card
                watCard = cardOffice.create(id, 5);
                continue; // Loop back in order to retry-without-random-wait
            }
            break; // In all cases except "Lost", exit "retry-without-random-wait" loop
        }
    }

    delete giftCard;
    delete watCard;
}

Student::Student( Printer & prt, NameServer & nameServer, WATCardOffice & cardOffice, Groupoff & groupoff,
            unsigned int id, unsigned int maxPurchases ) : printer(prt), nameServer(nameServer),
            cardOffice(cardOffice), groupoff(groupoff), id(id), maxPurchases(maxPurchases) {}
