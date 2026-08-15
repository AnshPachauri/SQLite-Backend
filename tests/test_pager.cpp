#include "pager.h"
#include <cassert>
#include <iostream>

int main() {
    // Step 1: clean slate — remove any leftover test file from a previous run
    remove("test.db");

    // Step 2: create a Pager, allocate a page, write a recognizable byte
    {
        Pager pager("test.db");
        PageNum page = pager.allocatePage();
        uint8_t* buffer = pager.getPage(page);
        buffer[0] = 42;
        // pager goes out of scope here -> destructor runs -> flushAll() -> close()
    }

    // Step 3: fresh Pager on the same file, verify the byte survived
    {
        Pager pager("test.db");
        uint8_t* buffer = pager.getPage(0);
        assert(buffer[0] == 42);
        std::cout << "Test passed: page persisted across restart\n";
    }

    return 0;
}