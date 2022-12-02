#include <iostream>
#include <fstream>
#include <uPRNG.h>

#include "convert.h"

#include "config.h"

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
}
