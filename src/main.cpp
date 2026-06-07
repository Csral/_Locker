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

    // validate my configuration
    try {
        valid_arguments::validate_arguments(configuration);
    } catch (const valid_arguments::exceptions::invalid_setup& e_is) {
        std::cerr << e_is.what() << "\n" << "Please provide proper configuration. Pass --help for more info.\n";
        return 1;
    } catch (const valid_arguments::exceptions::missing_arguments& e_ma) {
        std::cerr << e_ma.what() << "\n";
        return 1;
    } catch (const valid_arguments::exceptions::app_invalid_argument& e_aia) {
        std::cerr << e_aia.what() << "\n";
        return 1;
    } catch (const valid_arguments::exceptions::validation_exception& e_ve) {
        std::cerr << e_ve.what() << "\n";
        return 1;
    } catch (const std::exception& e) {
        std::cerr << "Unknown error: " << e.what() << "\n";
        return 1;
    }

    std::cout << configuration << "\n";
    return 0;

};