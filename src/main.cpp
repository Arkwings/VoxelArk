#include "common.hpp"
#include "application.hpp"
#include "applicationTest.hpp"

int main(int argc, char* argv[]) {
    try {
        Application* VoxAdventure = new Application();
        VoxAdventure->Run();
        delete VoxAdventure;

        // ApplicationTest* test = new ApplicationTest();
        // test->Run();
        // delete test;
    }
    catch (const std::exception& e) {
        std::cerr << e.what() << '\n';
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}