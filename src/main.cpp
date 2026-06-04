#define LOCKER_APP_CONFIG_PRINTER_REQUIRED
#include "args.h"
#include "arg_parser.h"
#include <iostream>

int main(int argc, char* argv[]) {

    app_config configuration;
    try {
        configuration = argument_parser(argc, argv);
    } catch (const std::runtime_error& err) {
        std::cerr << "Error: " << err.what() << "\n" << "Use --help for usage.\n";
        return 1;
    } catch (const std::exception& err) {
        std::cerr << "Unhandled error: " << err.what() << "\n" << "Use --help for usage.\n";
        return 1;
    }

    std::cout << configuration << "\n";
    return 0;

};