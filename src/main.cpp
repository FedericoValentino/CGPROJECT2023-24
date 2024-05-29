#include <iostream>
#include "Engine/Project.hpp"
#include "Model/Include/Projectile.h"

// Mutex for synchronizing access to the critical section

int main() {

    Project app;

    try {
        app.run();
    }
    catch(const std::exception& e)
    {
        std::cerr<<e.what()<<std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;

}
