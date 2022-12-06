#include "bank.h"

#include <iostream>
#include <fstream>

Bank::Bank(unsigned int numStudents) : numStudents(numStudents), studentIdToBalance(numStudents, 0) {}

void Bank::deposit( unsigned int id, unsigned int amount ) {
    studentIdToBalance[id] += amount;
}

void Bank::withdraw( unsigned int id, unsigned int amount ) {
    if (studentIdToBalance[id] < amount) {
        for (;;) {
            if (studentIdToBalance[id] < amount) {
                _Accept(deposit);
            } else {
                break;
            }
        }
    }
    studentIdToBalance[id] -= amount;
}
