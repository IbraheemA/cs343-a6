#include "parent.h"
#include "printer.h"

#include <uPRNG.h>

Parent::Parent( Printer & prt, Bank & bank, unsigned int numStudents, unsigned int parentalDelay ) :
    printer(prt), bank(bank), numStudents(numStudents), parentalDelay(parentalDelay) {};

void Parent::main() {
    for (;;) {
        _Accept(~Parent) {
            break;
        } _Else {
            yield(parentalDelay);
            int randomAmount = prng(1,3);
            int randomStudent = prng(numStudents);
            printer.print(Printer::Kind::Parent, 'D', randomStudent, randomAmount); // XXX print before or after deposit?
            bank.deposit(randomStudent, randomAmount);
        }
    }
}
