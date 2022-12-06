#include <iostream>
#include <fstream>
#include <uPRNG.h>

#include "convert.h"

#include "config.h"

#include "bottlingplant.h"
#include "printer.h"
#include "vendingmachine.h"
#include "student.h"
#include "watcardoffice.h"
#include "nameserver.h"
#include "groupoff.h"
#include "bank.h"
#include "parent.h"

int main (int argc, char* argv[]) {

    const char * configFileName = "soda.config";    

    int processors = 1;

    try {
        switch (argc) {
            case 4: if ( strcmp( argv[3], "d" ) != 0 ) {	// default ?
                processors = convert( argv[3] ); if ( processors <= 0 ) throw 1; }
            case 3: if ( strcmp( argv[2], "d" ) != 0 ) {	// default ?
                int seed = convert( argv[2] ); if ( seed <= 0 ) throw 1; else set_seed(seed); }
            case 2:
                if ( strcmp( argv[1], "d" ) != 0 ) {	// default ?
                    configFileName = argv[1]; }
            case 1: break;								// use all defaults
            default: throw 1;    
        }
    } catch (...) {
        std::cerr << "Usage: " << argv[0] << " soda [ config-file | 'd' [ seed (> 0) | 'd' [ processors (> 0) | 'd' ] ] ]" << std::endl;
        exit( EXIT_FAILURE );
    }

    uProcessor p[processors - 1] __attribute__(( unused )); // create more kernel thread

    ConfigParms configParms;

    processConfigFile(configFileName, configParms);


    std::ofstream test_out{"t.out"};

    Printer printer{ configParms.numStudents, configParms.numVendingMachines, configParms.numCouriers };
    Bank * bank = new Bank{configParms.numStudents};
    WATCardOffice * watCardOffice = new WATCardOffice{printer, *bank, configParms.numCouriers};

    NameServer nameServer{printer, configParms.numVendingMachines, configParms.numStudents};
    Groupoff groupoff{printer, configParms.numStudents, configParms.sodaCost, configParms.groupoffDelay };

    BottlingPlant * bottlingPlant = new BottlingPlant{
        printer,
        nameServer,
        configParms.numVendingMachines,
        configParms.maxShippedPerFlavour,
        configParms.maxStockPerFlavour,
        configParms.timeBetweenShipments    
    };
    Parent * parent = new Parent{printer, *bank, configParms.numStudents, configParms.parentalDelay};

    std::vector<Student*> students;
    for (int i = 0; i < configParms.numStudents; ++i) {
        students.emplace_back(new Student{printer, nameServer, *watCardOffice, groupoff, i, configParms.maxPurchases});
    }

    std::vector<VendingMachine *> vendingMachines;
    for (int i = 0; i < configParms.numVendingMachines; ++i) {
        vendingMachines.emplace_back(new VendingMachine{printer, nameServer, i, configParms.sodaCost});
    }

    for (int i = 0; i < configParms.numStudents; ++i) {
        delete students[i];
    }

    delete bottlingPlant;


    for (int i = 0; i < configParms.numVendingMachines; ++i) {
        delete vendingMachines[i];
    }

    delete watCardOffice;

    delete parent;

    delete bank;

}
