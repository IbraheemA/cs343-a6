#include "watcardoffice.h"

#include <iostream>
#include <fstream>

void WATCardOffice::main() {
    printer.print(Printer::Kind::WATCardOffice, 'S');

    std::vector<Courier*> couriers;

    // std::ofstream test_out{"t.out", std::ios::app};
    // std::osacquire(/**/std::cout) << "office main time" << std::endl;

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

    // std::osacquire(/**/std::cout) << "creaed couriers" << std::endl;

    for (;;) {
        // std::osacquire(/**/std::cout) << "looper" << std::endl;
        _Accept (~WATCardOffice) {
            // std::osacquire(/**/std::cout) << "time to delete couriers" << std::endl;

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

            // std::osacquire(/**/std::cout) << "really time to delete couriers" << std::endl;
            for (;;) {
                _Accept(requestWork) {}
                _Else { break; };
            }

            // std::osacquire(/**/std::cout) << "nah nah now its time to delete couriers" << std::endl;
            while (!jobsAreAvailable.empty()) {
                jobsAreAvailable.signalBlock();
            }

            // std::osacquire(/**/std::cout) << "fr this time really time to delete couriers" << std::endl;
            for (int i = 0; i < numCouriers; ++i) {
                delete couriers[i];
            }

            // std::osacquire(/**/std::cout) << "office ded" << std::endl;
            break;
        } or _Accept(create || transfer || requestWork);
    }
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
    // std::cout << "really time to transfer " << sid << std::endl;
    Job *j = new Job{sid, amount, card};
    // std::cout << "transfer job created " << sid << std::endl;
    jobQueue.push(j);
    jobsAreAvailable.signal();
    printer.print(Printer::Kind::WATCardOffice, 'T', sid, amount);
    return j->result;
}

WATCardOffice::Job * WATCardOffice::requestWork() {
    // std::osacquire(/**/std::cout) << "enter requestwork" << std::endl;
    if (jobQueue.empty()) {
        // std::osacquire(/**/std::cout) << "block on requestwork" << std::endl;
        jobsAreAvailable.wait(); // Courier blocks until a new job is available;
    }
    if (jobQueue.empty()) {
        // std::osacquire(/**/std::cout) << "requestwork exceptional" << std::endl;
        return nullptr;
    }
    // std::osacquire(/**/std::cout) << "gonna pop now" << std::endl;
    Job * j = jobQueue.front();
    jobQueue.pop();
    printer.print(Printer::Kind::WATCardOffice, 'W');
    return j;
}

void WATCardOffice::Courier::main() {
    printer.print(Printer::Kind::Courier, id, 'S');
    for (;;) {
        // std::ofstream test_out{"t.out", std::ios::app};
        // Block and wait for work
        // std::osacquire(/**/std::cout) << "lets get employed" << std::endl;
        Job * j = office.requestWork();
        if (j == nullptr) {
            // std::osacquire(/**/std::cout) << "requestwork returned nullptr" << std::endl;
            break;
        }

        // std::osacquire(/**/std::cout) << "now let's withdraw" << std::endl;

        printer.print(Printer::Kind::Courier, id, 't', j->sid, j->amount);
        bank.withdraw(j->sid, j->amount); // Potentially blocking call to bank // XXX need to catch RendezvousFailure
        // std::osacquire(/**/std::cout) << "done withdrawing" << std::endl;

        // std::osacquire(/**/std::cout) << "unblocked" << std::endl;
        // std::osacquire(std::cout) << "unblocked" << std::endl;

        _Accept(~Courier) {
            // std::osacquire(/**/std::cout) << "accepted courier dtor" << std::endl;
            delete j;
            break;
        } _Else {
            // Potentially create new card
            WATCard* realCard = (j->existingCard == nullptr) ? new WATCard() : j->existingCard;

            // std::osacquire(/**/std::cout) << "after realCard" << std::endl;
            realCard->deposit(j->amount);

            if (prng(6) == 0) {
                // std::osacquire(/**/std::cout) << "card lost" << std::endl;
                // WATCard lost; deliver exception
                printer.print(Printer::Kind::Courier, id, 'L', j->sid);
                j->result.delivery(new Lost());
                if (j->existingCard == nullptr) {
                    delete realCard;
                }
            } else {
                // WATCard successfully delivered
                // std::osacquire(/**/std::cout) << "deliver realCard" << std::endl;
                j->result.delivery(realCard);
                printer.print(Printer::Kind::Courier, id, 'T', j->sid, j->amount);
            }
        }

        delete j;
    }
    // std::osacquire(/**/std::cout) << "courier done :)" << std::endl;
    printer.print(Printer::Kind::WATCardOffice, 'F');
}
