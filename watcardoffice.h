#pragma once

#include "watcard.h"
#include "bank.h"
#include "printer.h"

#include <vector>
#include <queue>

_Task WATCardOffice {
    struct Job {							// marshalled arguments and return future
        // Args
        const unsigned int sid;
        const unsigned int amount;
        WATCard * existingCard;

        WATCard::FWATCard result;			// return future

        // Ctor for "transfer" job
        Job( unsigned int sid, unsigned int amount, WATCard * existingCard)
            : sid{sid}, amount{amount}, existingCard(existingCard) {}

        // Ctor for "create" job
        Job( unsigned int sid, unsigned int amount) : Job{sid, amount, nullptr} {}
    };
    _Task Courier {
        void main();

        WATCardOffice & office;
        Bank & bank;
        Printer & printer;
        int id;

        public:
            Courier( WATCardOffice & office, Bank & bank, Printer & printer, int id )
            : office(office), bank(bank), printer{printer}, id{id} {}
    };					// communicates with bank

    void main() override;

    Printer & printer;
    Bank & bank;
    unsigned int numCouriers;

    uCondition jobsAreAvailable;

    std::queue<Job*> jobQueue;

public:
    _Event Lost {};							// lost WATCard
    WATCardOffice( Printer & prt, Bank & bank, unsigned int numCouriers );
    WATCard::FWATCard create( unsigned int sid, unsigned int amount )
        __attribute__(( warn_unused_result ));
    WATCard::FWATCard transfer( unsigned int sid, unsigned int amount, WATCard * card )
        __attribute__(( warn_unused_result ));
    Job * requestWork() __attribute__(( warn_unused_result ));
};
