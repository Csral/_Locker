#pragma once
#ifndef LOCKER_ARG_PARSER_H
#define LOCKER_ARG_PARSER_H

#include <string>
#include <vector>
#include <iostream>
#include "args.h"
#include "arguments_registry.h"

static void show_help(void) noexcept {

    std::cout << R"(

_Locker
========
Quickly lock and unlock files through a shuffler and reshuffler pipeline.

WARNING
-------
This tool is primarily intended for experimentation and lightweight file
obfuscation. Do not treat it as a replacement for proven cryptographic
software.

USAGE
-----
    _Locker INPUT_FILE [OPTIONS]

GENERAL OPTIONS
---------------
    --encode
        Encode the file.
        Default behaviour.

    --decode
        Decode the file.

    -o, --output FILE
        Output file path.

        Default:
            INPUT_FILE_encoded.<original_extension>

    -M, --memory SIZE{M,G}
        Set the maximum amount of RAM the locker may use.

        This value is used by Locker to determine how much
        file data may be processed in memory at a time.

        Note:
            This is not a strict process memory limit.
            Actual memory usage may exceed the specified
            value due to internal allocations and runtime
            overhead.

        Suffixes:
            M  Megabytes
            G  Gigabytes

        If no suffix is specified, M is assumed.

        Examples:
            -M 512
            -M 512M
            -M 2G

        Default:
            512M

    --app-warnings-off
        Disable application warnings.

        Warnings provide recommendations about potentially
        suboptimal configurations and performance-related settings.

        Default:
            Warnings are enabled.

    --app-warnings-on
        Enable application warnings.

        Warnings provide recommendations about potentially
        suboptimal configurations and performance-related settings.

        Default:
            Warnings are enabled.

    -h, --help
        Display this help message.

SHUFFLER
--------
    -s, --shuffler

    Transformation stage.

    Options:

        -a, --algorithm STRING
            Default: not

        -m, --modification-factor UNSIGNED
            Required.

        -c, --chunk-size UNSIGNED
            Default: 8

        --no-warnings
            Optional. Disabled by default.

        --disable
            Will not invoke shuffler.
            Optional. False by default.

    Available Algorithms:

)";

    arguments_registry::print_registry(arguments_registry::shuffler);

    std::cout << R"(

RESHUFFLER
----------
    -r, --reshuffler

    Rearrangement stage.

    Options:

        -a, --algorithm STRING
            Default: mirror

        -t, --type STRING
            Default: bit

        -c, --chunk-size UNSIGNED
            Default: 8

        -b, --block-size UNSIGNED
            Default: 4

        -S, --seed UNSIGNED
            Required.

        -x, --extargs {A,B,C,...}
            Optional.

            Examples:

                {1}
                {1,2,3}
                {1, 2, 3}
                {34,011,12}

            Rules:

                * Opening '{' required.
                * Closing '}' required.
                * Entries must be comma separated.
                * Spaces are ignored.
                * Unlimited values allowed.
        
        --disable
            Will not invoke shuffler.
            Optional. False by default.

RESHUFFLER TYPES
----------------

)";

    arguments_registry::print_registry(arguments_registry::reshuffler_type);

    std::cout << R"(

BIT RESHUFFLER ALGORITHMS
-------------------------

)";

    arguments_registry::print_registry(arguments_registry::reshuffler_bit);

    std::cout << R"(

CHUNK RESHUFFLER ALGORITHMS
---------------------------

)";

    arguments_registry::print_registry(arguments_registry::reshuffler_chunk);

    std::cout << R"(

NOTES
-----
* Some algorithms require additional parameters beyond the standard
  modification factor or seed.

* Such parameters are supplied through:

      -e {v1,v2,v3,...}

* If a selected algorithm requires extra arguments and they are not
  supplied, an exception will be thrown.

* Currently no chunk reshuffler algorithms exist.

EXAMPLES
--------

Encode with defaults:

    _Locker file.bin

Encode with a custom shuffler:

    _Locker file.bin
        -s -a state_machine -m 17

Encode and write to a custom output:

    _Locker file.bin
        -s -m 17
        -o output.bin

Mirror reshuffle:

    _Locker file.bin
        -s -m 17
        -r -a mirror -S 5

Mirror reshuffle with extended arguments:

    _Locker file.bin
        -s -m 17
        -r -a mirror -S 5
        -e {1,2,3,4}

)";
}

static inline size_t get_option_value_number(size_t ctr, const std::vector<std::string>& arg_list) {

    if (++ctr >= arg_list.size())
        throw std::invalid_argument("Exceeded maximum arguments provided.");

    std::string value = arg_list[ctr];

    if (value.empty()) 
        throw std::invalid_argument("Expected at least one value-argument. Found none");

    long long response = 0x00LL;
    size_t cutoff_pos = 0x00ULL;

    try {
        response = std::stoll(value, &cutoff_pos);

        if (cutoff_pos < value.size())
            throw std::invalid_argument("Error: No valid conversion could be performed.");

    } 
    
    #ifdef LOCKER_ARG_PARSER_DEBUG
    catch (const std::invalid_argument& e) {
        throw std::invalid_argument("Error: No valid conversion could be performed.\nE: " + std::string(e.what()));
    #else
    catch (const std::invalid_argument&) {
        throw std::invalid_argument("Error: No valid conversion could be performed.");
    #endif
    } 
    #ifdef LOCKER_ARG_PARSER_DEBUG
    catch (const std::out_of_range& e) {
        throw std::out_of_range("Error: Value is too large for unsigned long long.\nE: " + std::string(e.what()));
    #else
    catch (const std::out_of_range&) {
        throw std::out_of_range("Error: Value is too large for unsigned long long.");
    #endif
    }

    // There is a next argument and it doesn't start with a '-'
    return response;

};

static inline size_t get_option_value_u_number(size_t ctr, const std::vector<std::string>& arg_list) {

    if (++ctr >= arg_list.size())
        throw std::invalid_argument("Exceeded maximum arguments provided.");

    std::string value = arg_list[ctr];

    if (value.empty()) 
        throw std::invalid_argument("Expected at least one value-argument. Found none");

    if (arg_list[ctr][0] == '-') // Now, it is not empty but it can be a negative number or an option - both invalid.
        throw std::invalid_argument("Expected at least one value-argument. Found none");

    size_t response = 0x00ULL;
    size_t cutoff_pos = 0x00ULL;

    try {
        response = std::stoull(value, &cutoff_pos);

        if (cutoff_pos < value.size())
            throw std::invalid_argument("Error: No valid conversion could be performed.");

    }
    #ifdef LOCKER_ARG_PARSER_DEBUG
    catch (const std::invalid_argument& e) {
        throw std::invalid_argument("Error: No valid conversion could be performed.\nE: " + std::string(e.what()));
    #else
    catch (const std::invalid_argument&) {
        throw std::invalid_argument("Error: No valid conversion could be performed.");
    #endif
    } 
    #ifdef LOCKER_ARG_PARSER_DEBUG
    catch (const std::out_of_range& e) {
        throw std::out_of_range("Error: Value is too large for unsigned long long.\nE: " + e.what());
    #else
    catch (const std::out_of_range&) {
        throw std::out_of_range("Error: Value is too large for unsigned long long.");
    #endif
    }

    // There is a next argument and it doesn't start with a '-'
    return response;

};

static inline std::string get_option_value(size_t ctr, const std::vector<std::string>& arg_list) {

    if (++ctr >= arg_list.size())
        throw std::out_of_range("Exceeded maximum arguments provided.");

    std::string value = arg_list[ctr];

    if (value.empty()) 
        throw std::out_of_range("Expected at least one value-argument. Found none");

    if (arg_list[ctr][0] == '-') // Not value but argument!
        throw std::out_of_range("Expected at least one value-argument. Found none");

    // There is a next argument and it doesn't start with a '-'
    return value;

};

struct app_config argument_parser(int argc, char* argv[]) {

    if (argc < 2) throw std::runtime_error("Atleast one argument expected.");
    std::vector<std::string> arguments(argv + 1, argv + argc);

    // Any argument without '--' or '-' is an input variant.
    for (const auto& x : arguments)
        if (x == "-h" || x == "--help") {
            show_help(); // exit after help
            std::exit(0);
        }

    app_config configuration;

    bool shuffler_options_active = false;
    bool reshuffler_options_active = false;
    bool suboptions_active = false;

    // There is no requirement to capture a signed number yet.
    // long long _tmp_nxt_arg_num = 0x00LL;
    unsigned long long _tmp_nxt_arg_u_num = 0x00ULL;

    std::string _tmp_nxt_arg = ""; // temporary place to store next arguments.
    std::string who_caused = "";

    bool is_statement_parsing_again = false;

    /* Parse all arguments */
    for (size_t i = 0; i < arguments.size(); i++) {

        std::string arg = arguments[i];

        if (arg.empty())
            continue;

        if (arg[0] != '-') {
            // not a '--' or '-'. File name

            if (!configuration.input_file.empty()) {
                throw std::runtime_error("Unexpected argument " + arg + ". Filename already provided as: " + configuration.input_file);
            }

            configuration.input_file = arg;
            continue;
        }

        if (arg.size() < 2) {
            // so it was '-' but nothing follows it? 
            // We don't do that here
            throw std::runtime_error("An empty option is invalid: " + arg);
        }

        // '-' or '--': Short or long options.

        if (suboptions_active) {

            if (arg == "--chunk-size" || arg == "-c") {

                // Common
                try {
                    _tmp_nxt_arg_u_num = get_option_value_u_number(i, arguments);
                } catch (const std::invalid_argument& e) {
                    throw std::runtime_error("Invalid argument provided for " + arg + ": " + e.what());
                } catch (const std::out_of_range& e) {
                    throw std::runtime_error("Maximum numerical range exceeded for " + arg + ": " + e.what());
                }

                // belongs to shuffler
                if (shuffler_options_active)
                    configuration.shuffler_opts.chunk_size = _tmp_nxt_arg_u_num;
                else if (reshuffler_options_active)
                    configuration.reshuffler_opts.chunk_size = _tmp_nxt_arg_u_num;
                else
                    throw std::runtime_error("Internal error.");

                i++;

            } else if (arg == "--algorithm" || arg == "-a") {

                // Common

                try {
                    
                    _tmp_nxt_arg = get_option_value(i, arguments);
                    
                    if (shuffler_options_active) {

                        // Change aliases to actual names.
                        try {
                            _tmp_nxt_arg = valid_arguments::parse_shuffler_args(_tmp_nxt_arg);
                        } catch (const std::runtime_error&) {
                            throw std::runtime_error("Invalid argument provided for shuffler: " + _tmp_nxt_arg);
                        }
                        
                        configuration.shuffler_opts.algorithm = _tmp_nxt_arg;

                    } else if (reshuffler_options_active) {

                        // Reshuffler algorithms have no aliases - atleast right now.
                        // Also, the algorithm may belong to bit-type or chunk-type and
                        // the type may change later-on in the argument parsing.
                        // Validating the algorithm is best-done at end.

                        if (!(valid_arguments::reshuffler_bit.count(_tmp_nxt_arg) + valid_arguments::reshuffler_chunk.count(_tmp_nxt_arg)))
                            throw std::runtime_error("Invalid argument provided for reshuffler: " + _tmp_nxt_arg);

                        configuration.reshuffler_opts.algorithm = _tmp_nxt_arg;

                    } else {
                        throw std::runtime_error("Internal error");
                    }

                } catch (const std::out_of_range&) {
                    throw std::runtime_error("--algorithm expects an algorithm to be provided. Found none.");
                }

                i++;

            } else if (arg == "--modification-factor" || arg == "-m") {
            
                // specific to shuffler

                try {
                    _tmp_nxt_arg_u_num = get_option_value_u_number(i, arguments);
                } catch (const std::invalid_argument& e) {
                    throw std::runtime_error("Invalid argument provided for " + arg + ": " + e.what());
                } catch (const std::out_of_range& e) {
                    throw std::runtime_error("Maximum numerical range exceeded for " + arg + ": " + e.what());
                }

                if (shuffler_options_active)
                    configuration.shuffler_opts.modification_factor = _tmp_nxt_arg_u_num;
                else if (reshuffler_options_active)
                    throw std::runtime_error(arg + " is only a suboption for shuffler. Used with reshuffler.");
                else
                    throw std::runtime_error("Internal error.");

                i++;

            } else if (arg == "--no-warnings") {

                if (shuffler_options_active)
                    configuration.shuffler_opts.warnings = false;
                else if (reshuffler_options_active)
                    throw std::runtime_error(arg + " is only a suboption for shuffler and not supported/required by reshuffler yet. \nUsed with reshuffler.");
                else
                    throw std::runtime_error("Internal error.");

            } else if (arg == "--disable") {

                if (shuffler_options_active)
                    configuration.shuffler_opts.disabled = true;
                else if (reshuffler_options_active)
                    configuration.reshuffler_opts.disabled = true;
                else
                    throw std::runtime_error("Internal error.");

            } else if (arg == "--type" || arg == "-t") {
                
                // specific to reshuffler

                try {
                    
                    _tmp_nxt_arg = get_option_value(i, arguments);
                    
                    if (shuffler_options_active) {

                        throw std::runtime_error(arg + " is a suboption for reshuffler. Used with shuffler.");

                    } else if (reshuffler_options_active) {

                        // Reshuffler algorithms have no aliases - atleast right now.
                        // Also, the algorithm may belong to bit-type or chunk-type and
                        // the type may change later-on in the argument parsing.
                        // Validating the algorithm is best-done at end.

                        if (!valid_arguments::reshuffler_type.count(_tmp_nxt_arg))
                            throw std::runtime_error("Invalid type provided for reshuffler: " + _tmp_nxt_arg);

                        configuration.reshuffler_opts.type = _tmp_nxt_arg;

                    } else {
                        throw std::runtime_error("Internal error");
                    }

                } catch (const std::out_of_range&) {
                    throw std::runtime_error("--type expects a type to be provided. Found none.");
                }

                i++;

            } else if (arg == "--block-size" || arg == "-b") {

                // specific to reshuffler

                try {
                    _tmp_nxt_arg_u_num = get_option_value_u_number(i, arguments);
                } catch (const std::invalid_argument& e) {
                    throw std::runtime_error("Invalid argument provided for " + arg + ": " + e.what());
                } catch (const std::out_of_range& e) {
                    throw std::runtime_error("Maximum numerical range exceeded for " + arg + ": " + e.what());
                }

                if (shuffler_options_active)
                    throw std::runtime_error(arg + " is only a suboption for reshuffler. Used with shuffler.");
                else if (reshuffler_options_active)
                    configuration.reshuffler_opts.block_size = _tmp_nxt_arg_u_num;
                else
                    throw std::runtime_error("Internal error.");

                i++;

            } else if (arg == "--seed" || arg == "-S") {

                // specific to reshuffler

                try {
                    _tmp_nxt_arg_u_num = get_option_value_u_number(i, arguments);
                } catch (const std::invalid_argument& e) {
                    throw std::runtime_error("Invalid argument provided for " + arg + ": " + e.what());
                } catch (const std::out_of_range& e) {
                    throw std::runtime_error("Maximum numerical range exceeded for " + arg + ": " + e.what());
                }

                if (shuffler_options_active)
                    throw std::runtime_error(arg + " is only a suboption for reshuffler. Used with shuffler.");
                else if (reshuffler_options_active)
                    configuration.reshuffler_opts.seed = _tmp_nxt_arg_u_num;
                else
                    throw std::runtime_error("Internal error.");

                i++;

            } else if (arg == "--extargs" || arg == "-x") {

                // specific to reshuffler

            } else {

                suboptions_active = false;
                i--;
                // ask global space to inspect once

                if (shuffler_options_active) who_caused = "Shuffler";
                else if (reshuffler_options_active) who_caused = "Reshuffler";
                else who_caused = "";

                // let global space be aware of what happened.
                is_statement_parsing_again = true;

            }

        } else {

            if (arg == "--shuffler" || arg == "-s") {

                suboptions_active = true;
                shuffler_options_active = true;
                reshuffler_options_active = false;

            } else if (arg == "--reshuffler" || arg == "-r") {

                suboptions_active = true;
                shuffler_options_active = false;
                reshuffler_options_active = true;

            } else if (arg == "--output" || arg == "-o") {

                try {
                    
                    _tmp_nxt_arg = get_option_value(i, arguments);
                    configuration.output_file = _tmp_nxt_arg;

                } catch (const std::out_of_range&) {
                    throw std::runtime_error("--output expects a the output file name. Found none.");
                }

                i++;

            } else if (arg == "-M" || arg == "--memory") {

                try {

                    std::size_t multiplication_factor = 1024ULL * 1024ULL; // MB
                    std::size_t memory_allowed_size = 0;
                    std::size_t numeric_convert_cutoff_pos = 0x00ULL;

                    _tmp_nxt_arg = get_option_value(i, arguments);
                    char last_char = _tmp_nxt_arg.back();

                    if ((last_char < 48) || (last_char > 57)) // not a number - clear it.
                        _tmp_nxt_arg.resize(_tmp_nxt_arg.size() - 1);

                    if (last_char == 'G' || last_char == 'g') // We don't care about anything else.
                        multiplication_factor *= 1024ULL;
                    else if (last_char == 'M' || last_char == 'm') {}
                    else if ((last_char > 47) && (last_char < 58)) {}
                    else
                        throw std::runtime_error("Invalid size for ram usage specified: " + std::string(1, last_char)); // Well, looks good for error message.

                    try {
                        memory_allowed_size = std::stoull(_tmp_nxt_arg, &numeric_convert_cutoff_pos);

                        if (numeric_convert_cutoff_pos < _tmp_nxt_arg.size())
                            throw std::runtime_error("Error: Invalid numeric value for ram usage: " + _tmp_nxt_arg);

                        if (memory_allowed_size > (locker_limits_MAX_WORKSPACE_BYTES / multiplication_factor))
                            throw std::runtime_error("Too much ram usage allowed. Maximum allowed: " + std::to_string((locker_limits_MAX_WORKSPACE_BYTES / multiplication_factor)) + std::string(1, last_char));

                        memory_allowed_size *= multiplication_factor;

                    }
                    #ifdef LOCKER_ARG_PARSER_DEBUG
                    catch (const std::invalid_argument& e) {
                        throw std::runtime_error("Error: Invalid numeric value for ram usage: " + _tmp_nxt_arg + "\nE: " + std::string(e.what()));
                    #else
                    catch (const std::invalid_argument&) {
                        throw std::runtime_error("Error: Invalid numeric value for ram usage: " + _tmp_nxt_arg);
                    #endif
                    }
                    #ifdef LOCKER_ARG_PARSER_DEBUG
                    catch (const std::out_of_range& e) {
                        throw std::runtime_error("Error: Value for ram usage is too large: " + _tmp_nxt_arg + "\nE: " + std::string(e.what()));
                    #else
                    catch (const std::out_of_range&) {
                        throw std::runtime_error("Error: Value for ram usage is too large: " + _tmp_nxt_arg);
                    #endif
                    }

                    configuration.memory_usage = memory_allowed_size;

                } catch (const std::out_of_range&) {
                    throw std::runtime_error(arg + " expects a memory usage limit.");
                }

                i++;

            } else if (arg == "--app-warnings-on") {
                configuration.warnings = true;
            } else if (arg == "--app-warnings-off") {
                configuration.warnings = false;
            } else if (arg == "--decode" || arg == "-d") {

                if (configuration.is_encoder)
                    configuration.is_encoder = false;

                // if someone passes --decode many times, it is simply ignored.

            } else if (arg == "--encode" || arg == "-e") {

                if (!configuration.is_encoder)
                    throw std::runtime_error("You cannot decode and encode in the same run.");

                // if someone passes --encode for no reason, it is simply ignored.

            } else {
                
                if (is_statement_parsing_again) {
                    // so suboptions raised this request
                    throw std::runtime_error("Unknown " + who_caused + " option: " + arg);
                }

                throw std::runtime_error("Unknown option provided: " + arg + "");
            }

            // we handled the argument successfully - clear any error flags
            is_statement_parsing_again = false;

        }

    }

    return configuration;

};

#endif