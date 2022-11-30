#include <map>

_Monitor Bank {
    unsigned int numStudents;
    vector<int> studentIdToBalance;
  public:
    Bank( unsigned int numStudents );
    void deposit( unsigned int id, unsigned int amount );
    void withdraw( unsigned int id, unsigned int amount );
};
