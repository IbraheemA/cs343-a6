#pragma once

#include <string>

_Monitor Printer {
	unsigned int numStudents;
	unsigned int numVendingMachines;
	unsigned int numCouriers;

	std::string *printBuffer;

	void flushBuffer(int index) {
		if (printBuffer[index] != "") {
			for (std::string s: printBuffer) {
				std::cout << s << "\t";
				s = "";
			}
			std::cout << std::endl;
		}
	}

	int getIndex(Kind kind, unsigned int lid) {
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
	}
  public:
	enum Kind { Parent, Groupoff, WATCardOffice, NameServer, Truck, BottlingPlant, Student, Vending, Courier };
	Printer( unsigned int numStudents, unsigned int numVendingMachines, unsigned int numCouriers ) :
		numStudents{numStudents}, numVendingMachines{numVendingMachines}, numCouriers{numCouriers} {
			printBuffer = new string[6 + numStudents + numVendingMachines + numCouriers];

			for (string s: printBuffer) {
				s = "";
			}
		}
	~Printer() {
		delete printBuffer;
	}
	void print( Kind kind, char state ) {
		flushBuffer(kind);
		printBuffer[kind] = state;
	}
	void print( Kind kind, char state, unsigned int value1 ) {
		flushBuffer(kind);
		printBuffer[kind] = state + value1;
	}
	void print( Kind kind, char state, unsigned int value1, unsigned int value2 ) {
		flushBuffer(kind);
		printBuffer[kind] = state + " " + value1 + "," + value2;
	}
	void print( Kind kind, unsigned int lid, char state ) {
		int index = getIndex(kind, lid)
		flushBuffer(index);
		printBuffer[index] = state;
	}
	void print( Kind kind, unsigned int lid, char state, unsigned int value1 ) {
		int index = getIndex(kind, lid)
		flushBuffer(index);
		printBuffer[index] = state + value1;
	}
	void print( Kind kind, unsigned int lid, char state, unsigned int value1, unsigned int value2 ) {
		int index = getIndex(kind, lid)
		flushBuffer(index);
		printBuffer[index] = state + " " + value1 + "," + value2;
	}
};
