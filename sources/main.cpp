#include <cstdlib>
#include <exception>
#include <iostream>

import rexengine.viewport;
import rexengine.console;

int main(int argc, char** argv) {
    try {
        rexengine::setAppName("JURASSIC_ROAR");
        rexengine::VIEWPORT();
        rexengine::VIEWPORT().run();

        return EXIT_SUCCESS;
    }
    catch(std::exception& e) {
        std::cerr << "FATAL ERROR: " << e.what() << std::endl;
        
        return EXIT_FAILURE;
    }
    catch(...) {
        std::cerr << "FATAL ERROR: UNKNOWN" << std::endl;

        return EXIT_FAILURE;
    }
}