#include "system.h"

int main ( int argc, char** argv )
{
    System system;
    if (!system.init()) {
        return 1;
    }

    system.run();
    return 0;
} // END MAIN
