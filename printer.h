#pragma once

#include <string> 
#include <iostream>

_Monitor Printer {
	unsigned int numStudents;
	unsigned int numVendingMachines;
	unsigned int numCouriers;

	class PrintEntry {
		char state;
		unsigned int value1;
		unsigned int value2;
		unsigned int numArgs;

		public:
			PrintEntry( char state) : state{state}, numArgs{1} {}
			PrintEntry( char state, unsigned int value1 ) : state{state}, value1{value1}, numArgs{2} {}
			PrintEntry( char state, unsigned int value1, unsigned int value2 ) : state{state}, value1{value1}, value2{value2}, numArgs{3} {}

			void print() {
				std::cout << state;
				if (numArgs > 1) {
					std::cout << value1;
				}
				if (numArgs > 2) {
					std::cout << "," << value2;
				}
			}
	};

	PrintEntry **printBuffer;

	void flushBuffer(int index) {
		if (printBuffer[index] != nullptr) {
			for (int i = 0; i < 6 + numStudents + numVendingMachines + numCouriers; i++) {
				if (printBuffer[i] != nullptr) {
					printBuffer[i]->print();
					std::cout << "\t";
					delete printBuffer[i];
					printBuffer[i] = nullptr;
				}
			}
			std::cout << std::endl;
		}
	}

	int getIndex(int kind, unsigned int lid) {
		int offset = 6 + lid;
		switch (kind)
		{
		case Student:
			break;
		case Vending:
			offset += numStudents;
			break;
		case Courier:
			offset += numVendingMachines;
			break;
		default:
			break;
		}
		return offset;
	}
  public:
	enum Kind { Parent, Groupoff, WATCardOffice, NameServer, Truck, BottlingPlant, Student, Vending, Courier };
	Printer( unsigned int numStudents, unsigned int numVendingMachines, unsigned int numCouriers ) :
		numStudents{numStudents}, numVendingMachines{numVendingMachines}, numCouriers{numCouriers} {
			printBuffer = new PrintEntry*[6 + numStudents + numVendingMachines + numCouriers];

			for (int i = 0; i < 6 + numStudents + numVendingMachines + numCouriers; i++) {
				// std::cout <<  << "\t"; 
				printBuffer[i] = nullptr;
			}
		}
	~Printer() {
		for (int i = 0; i < 6 + numStudents + numVendingMachines + numCouriers; i++) {
			if (printBuffer[i] != nullptr) {
				delete printBuffer[i];
			}
		}

		delete [] printBuffer;
	}

	void print( Kind kind, char state ) {
		flushBuffer(kind);
		printBuffer[kind] = new PrintEntry(state);
	}
	void print( Kind kind, char state, unsigned int value1 ) {
		flushBuffer(kind);
		printBuffer[kind] = new PrintEntry(state, value1);
	}
	void print( Kind kind, char state, unsigned int value1, unsigned int value2 ) {
		flushBuffer(kind);
		printBuffer[kind] = new PrintEntry(state, value1, value2);
	}
	void print( Kind kind, unsigned int lid, char state ) {
		int index = getIndex(kind, lid);
		flushBuffer(index);
		printBuffer[index] = new PrintEntry(state);
	}
	void print( Kind kind, unsigned int lid, char state, unsigned int value1 ) {
		int index = getIndex(kind, lid);
		flushBuffer(index);
		printBuffer[index] = new PrintEntry(state, value1);
	}
	void print( Kind kind, unsigned int lid, char state, unsigned int value1, unsigned int value2 ) {
		int index = getIndex(kind, lid);
		flushBuffer(index);
		printBuffer[index] = new PrintEntry(state, value1, value2);
	}
};
