_Task BottlingPlant {
    Printer & prt;
    NameServer & nameServer;
    unsigned int numVendingMachines;
    unsigned int maxShippedPerFlavour;
    unsigned int maxStockPerFlavour;
    unsigned int timeBetweenShipments;

    uCondition shipment;

    unsigned int *nextShipment;

    void main() {
      Truck truck = Truck(prt, nameServer, this, numVendingMachines, maxStockPerFlavour);

      for (int i = 0; i < 4; i++) {
        nextShipment[i] = prng(maxShippedPerFlavour);
      }

      for (;;) {
        _Accept(getShipment) {
          yield(timeBetweenShipments);
          for (int i = 0; i < 4; i++) {
            nextShipment[i] = prng(maxShippedPerFlavour);
          }
        }
      }
    }
  public:
    enum Flavours { Beef, BuhrBrew, meme1, meme2 };	// flavours of soda (YOU DEFINE)
    _Event Shutdown {};					// shutdown plant
    BottlingPlant( Printer & prt, NameServer & nameServer, unsigned int numVendingMachines,
                unsigned int maxShippedPerFlavour, unsigned int maxStockPerFlavour,
                unsigned int timeBetweenShipments ) :
                prt{prt}, nameServer{nameServer}, numVendingMachines{numVendingMachines},
                maxShippedPerFlavour{maxShippedPerFlavour}, maxStockPerFlavour{maxStockPerFlavour},
                timeBetweenShipments{timeBetweenShipments} {
                  nextShipment = new unsigned int[4]; // Length of Flavours
                }
    void getShipment( unsigned int cargo[] ) {
      // I think we might have to add a conditional lock here
      for (int i = 0; i < 4; i++) {
        cargo[i] = nextShipment[i];
      }
    }
};