// C headers
#include <cstdio>                          // for EOF
#include <cstdlib>                         // for exit, EXIT_SUCCESS, EXIT_F...
#include <getopt.h>                        // for getopt_long, required_argu...

// C++ headers
#include <iostream>                        // for operator<<, endl, basic_os...
#include <memory>                          // for shared_ptr
#include <stdexcept>                       // for runtime_error
#include <string>                          // for string
#include <utility>                         // for move, pair, make_pair

// Application headers
#include "Core.hpp"                        // for Core
#include "interface/InterfaceFactory.hpp"  // for InterfaceFactory
#include "persistence/StorageFactory.hpp"  // for StorageFactory


//-----------------------------------------------------------------------------

void printUsage();
void printUsage()
{
    std::cout << "Usage: KVPStorage [OPTION]\n" << std::endl;
    std::cout << "Store and retrieve settings and other string values." << std::endl;
    std::cout << "  -d, --database FILENAME       database file name" << std::endl;
    std::cout << "  -u, --ui   [SCRIPT|web|gui]   execute commands from SCRIPT file,\n"
              << "                                display a Webbrowser instead of command line,\n"
              << "                                display a Graphical User Interface.\n"
              << "                                If no --ui is given, the program executes at Command Line"
              << std::endl;
    std::cout << "  -h, --help                    print this help" << std::endl;
}

//-----------------------------------------------------------------------------

std::pair<std::string, std::string> getArgs(int argc, char *argv[]);
std::pair<std::string, std::string> getArgs(int argc, char *argv[])
{
    std::string database{};
    std::string uiname{};

    constexpr const char DATABASE_SHORTOPT{'d'};
    constexpr const char UI_SHORTOPT{'u'};
    constexpr const char HELP_SHORTOPT{'h'};

    char shortopts[] = "d:s:h";
    struct option longopts[] = {
        { "database", required_argument, 0, DATABASE_SHORTOPT },
        { "ui", required_argument, 0, UI_SHORTOPT },
        { "help", no_argument, 0, HELP_SHORTOPT },
        { 0, 0, 0, 0 }
    };

    int option = 0;
	do {
        option = getopt_long(argc, argv, shortopts, longopts, 0);
        switch (option)
        {
        case DATABASE_SHORTOPT:
            database = optarg;
            break;
        case UI_SHORTOPT:
            uiname = optarg;
            break;
        case HELP_SHORTOPT:
        case '?':
            printUsage();
            exit(EXIT_SUCCESS);
            break;
        default:
            // std::cerr << "Invalid option " << static_cast<char>(option) << std::endl;
            // printUsage();
            // exit(EXIT_FAILURE);
            break;
        }
    } while (option != EOF);

    return std::make_pair(database, uiname);
}

//-----------------------------------------------------------------------------

int main(int argc, char *argv[])
{
    std::cout << "Key value pair" << std::endl;

    std::pair<std::string, std::string> arguments{ getArgs(argc, argv) };
    std::string database{ arguments.first };
    if (database.empty()) {
        printUsage();
    }

    std::string uiname{ arguments.second };
    std::shared_ptr<Interface> interface{ InterfaceFactory::create(uiname, argc, argv) };

    try {
        std::shared_ptr<Storage> storage{ StorageFactory::create(database) };
        Core core{ std::move(interface), std::move(storage) };
        core.loop();
    } catch (const std::runtime_error &ex) {
        std::cerr << ex.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

//-----------------------------------------------------------------------------
