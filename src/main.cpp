#include "pager.h"
#include <iostream>

using namespace std;

int main(int argc, char** argv) {
    if (argc < 2) {
        cerr << "Usage: " << argv[0] << " <database file>\n";
        return 1;
    }
    Pager pager(argv[1]);
    cout << "Opened database. Nothing to do yet — Schema/BTree come next.\n";
    return 0;
}