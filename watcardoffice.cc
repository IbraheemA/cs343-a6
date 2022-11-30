#include "watcardoffice.h"

WATCardOffice::WATCardOffice( Printer & prt, Bank & bank, unsigned int numCouriers )
    : printer{prt}, bank{bank}, couriers(numCouriers) {}

WATCard::FWATCard WATCardOffice::create( unsigned int sid, unsigned int amount ) {
    Job *j = new Job{sid, amount};
    jobQueue.push(j);
    return j->result;
}

WATCard::FWATCard WATCardOffice::transfer( unsigned int sid, unsigned int amount, WATCard * card ) {
    Job *j = new Job{sid, amount, card};
    jobQueue.push(j);
    return j->result;
}

WATCardOffice::Job * WATCardOffice::requestWork() {
    if (jobQueue.empty()) {
        _Accept(create || transfer); // Courier blocks until a new job is available
    }
    Job * j = jobQueue.front();
    jobQueue.pop();
    return j;
}

WATCardOffice::Courier::main() {
    for (;;) {
        // Block and wait for work
        Job * j = office.requestWork();

        // Potentially create new card
        WATCard* realCard = (j->existingCard == nullptr) ? new WATCard() : j->existingCard;

        // Always request money from bank and load onto card once received
        bank.withdraw(j->sid, j->amount); // Potentially blocking call to bank // XXX need to catch RendezvousFailure
        realCard->deposit(j->amount);

        if (prng(6) == 0) {
            // WATCard lost; deliver exception
            j->result.delivery(new Lost());
            delete realCard;
        } else {
            // WATCard successfully delivered
            j->result.delivery(realCard);
        }

        delete j;
    }
}
