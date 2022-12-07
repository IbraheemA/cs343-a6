#include "watcardoffice.h"

#include <iostream>
#include <fstream>

void WATCardOffice::main() {
    printer.print(Printer::Kind::WATCardOffice, 'S');

    std::vector<Courier*> couriers;

    for (int i = 0; i < numCouriers; ++i) {
        couriers.push_back(
            new Courier(
                *this,
                bank,
                printer,
                i
            )
        );
    }

    for (;;) {
        _Accept (~WATCardOffice) {

            while (!jobQueue.empty()) {
                Job * j = jobQueue.front();
                jobQueue.pop();
                // if (j->existingCard != nullptr) {
                //     delete j->existingCard;
                // }
                delete j;
            }

            while (!jobsAreAvailable.empty()) {
                jobsAreAvailable.signalBlock();
            }

            for (;;) {
                _Accept(requestWork) {}
                _Else { break; };
            }

            while (!jobsAreAvailable.empty()) {
                jobsAreAvailable.signalBlock();
            }

            for (int i = 0; i < numCouriers; ++i) {
                delete couriers[i];
            }

            break;
        } or _Accept(create || transfer || requestWork);
    }
    printer.print(Printer::Kind::WATCardOffice, 'F');
}

WATCardOffice::WATCardOffice( Printer & prt, Bank & bank, unsigned int numCouriers )
    : printer{prt}, bank{bank}, numCouriers{numCouriers} {}

WATCard::FWATCard WATCardOffice::create( unsigned int sid, unsigned int amount ) {
    Job *j = new Job{sid, amount};
    jobQueue.push(j);
    jobsAreAvailable.signal();
    printer.print(Printer::Kind::WATCardOffice, 'C', sid, amount);
    return j->result;
}

WATCard::FWATCard WATCardOffice::transfer( unsigned int sid, unsigned int amount, WATCard * card ) {
    Job *j = new Job{sid, amount, card};
    jobQueue.push(j);
    jobsAreAvailable.signal();
    printer.print(Printer::Kind::WATCardOffice, 'T', sid, amount);
    return j->result;
}

WATCardOffice::Job * WATCardOffice::requestWork() {
    if (jobQueue.empty()) {
        jobsAreAvailable.wait(); // Courier blocks until a new job is available;
    }
    if (jobQueue.empty()) {
        return nullptr;
    }
    Job * j = jobQueue.front();
    jobQueue.pop();
    printer.print(Printer::Kind::WATCardOffice, 'W');
    return j;
}

void WATCardOffice::Courier::main() {
    printer.print(Printer::Kind::Courier, id, 'S');
    for (;;) {
        // Block and wait for work
        Job * j = office.requestWork();
        if (j == nullptr) {
            break;
        }


        printer.print(Printer::Kind::Courier, id, 't', j->sid, j->amount);
        bank.withdraw(j->sid, j->amount); // Potentially blocking call to bank // XXX need to catch RendezvousFailure

        _Accept(~Courier) {
            delete j;
            break;
        } _Else {
            // Potentially create new card
            WATCard* realCard = (j->existingCard == nullptr) ? new WATCard() : j->existingCard;

            realCard->deposit(j->amount);

            if (prng(6) == 0) {
                // WATCard lost; deliver exception
                j->result.exception(new Lost);
                printer.print(Printer::Kind::Courier, id, 'L', j->sid);
                delete realCard;
            } else {
                // WATCard successfully delivered
                j->result.delivery(realCard);
                printer.print(Printer::Kind::Courier, id, 'T', j->sid, j->amount);
            }
        }

        delete j;
    }
    printer.print(Printer::Kind::Courier, 'F');
}
