// Copyright (C) 2026 Csral chaturyasral@gmail.com
// SPDX-License-Identifier: GPL-3.0

#pragma once
#ifndef U_LOCKER_ARGS_H
#define U_LOCKER_ARGS_H

#include <string>
#include <vector>
#include <optional>
#include <unordered_set>
#include <unordered_map>
#include <stdexcept>

constexpr std::size_t locker_limits_MAX_WORKSPACE_BYTES = SIZE_MAX / 8;
constexpr std::size_t locker_limits_minimum_ram_bytes = 2*1024*1024; // 2 MB

struct shuffler_options {

    bool warnings = true;
    std::string algorithm = "not";
    std::size_t chunk_size = 8ULL;
    std::optional<std::size_t> modification_factor;

    bool disabled = false;

};

struct reshuffler_options {

    std::string algorithm = "mirror";
    std::string type = "bit";
    std::size_t chunk_size = 8ULL;
    std::size_t block_size = 4ULL;
    std::optional<std::size_t> seed;
    std::vector<unsigned long> extargs;

    bool disabled = false;

};

struct app_config {

    std::string input_file;
    std::string output_file;
    bool is_encoder = true;

    bool warnings = true;

    std::size_t memory_usage = 512ULL * 1024ULL * 1024ULL;

    struct shuffler_options shuffler_opts;
    struct reshuffler_options reshuffler_opts;

};

namespace valid_arguments {

    namespace exceptions {

        class validation_exception : public std::runtime_error {
            public:
            explicit validation_exception(const std::string& msg) : std::runtime_error("Validation error: " + msg) {};
        };

        class invalid_setup : public std::runtime_error {
            public:
            explicit invalid_setup(const std::string& submodule, const std::string& msg) : std::runtime_error("Invalid argument setup. Error by " + submodule + "\nError: " + msg) {};
        };

        class missing_arguments : public std::runtime_error {
            public:
            explicit missing_arguments(const std::string& msg) : std::runtime_error("Missing required argument: " + msg) {};
        };

        class app_invalid_argument : public std::invalid_argument {
            public:
            explicit app_invalid_argument(const std::string& msg) : std::invalid_argument("Invalid argument passed: " + msg) {};
        };

    };

    inline const std::unordered_map<std::string, std::string> shuffler = {

        { "not", "not" },
        { "state_machine", "state_machine" },
        { "sm", "state_machine" },
        { "pp", "prefix_parity" },
        { "prefix_parity", "prefix_parity" }

    };

    inline const std::unordered_set<std::string> reshuffler_bit = {
        "mirror"
    };

    inline const std::unordered_set<std::string> reshuffler_chunk = {
        
    };

    inline const std::unordered_set<std::string> reshuffler_type = {
        "bit", "chunk"
    };

    inline std::string parse_shuffler_args(const std::string& str) {
            
        auto it = valid_arguments::shuffler.find(str);

        if (it == valid_arguments::shuffler.end())
            throw std::runtime_error("Invalid shuffler algorithm: " + str);

        return it->second;

    }

    inline void validate_arguments(struct app_config& conf) {

        if (conf.input_file.empty())
            throw valid_arguments::exceptions::missing_arguments("Input file cannot be empty!");

        if (conf.output_file.empty()) {

            // decide a name for output file if empty.

            std::string modifier_name = (conf.is_encoder) ? "_encoded" : "_decoded";

            size_t extension_position = conf.input_file.rfind('.', std::string::npos);
            if (extension_position == std::string::npos) {
                conf.output_file = conf.input_file + modifier_name;
            } else {
                conf.output_file = conf.input_file.substr(0x00U, extension_position) + modifier_name + conf.input_file.substr(extension_position, std::string::npos);
            }

        }

        if (conf.memory_usage == 0) {
            throw valid_arguments::exceptions::app_invalid_argument("Allowed memory usage cannot be 0 bytes.");
        }

        if (!conf.shuffler_opts.modification_factor.has_value()) {
            throw valid_arguments::exceptions::missing_arguments("Modification factor must be provided for shuffler.");
        }

        if (*conf.shuffler_opts.modification_factor > conf.shuffler_opts.chunk_size) {
            // we literally do nothing within a chunk
            throw valid_arguments::exceptions::invalid_setup("Shuffler", "Modification factor cannot be greater than chunk size. The shuffler will do nothing.\nChunk size: " + std::to_string(conf.shuffler_opts.chunk_size) + "\nModification factor: " + std::to_string(*conf.shuffler_opts.modification_factor));
        }

        if (conf.shuffler_opts.chunk_size == 0)
            throw valid_arguments::exceptions::invalid_setup("Shuffler", "Chunk size cannot be zero.");

        if (*conf.shuffler_opts.modification_factor == 0)
            throw valid_arguments::exceptions::app_invalid_argument("Modification factor cannot be zero.");

        if (!conf.reshuffler_opts.seed.has_value())
            throw valid_arguments::exceptions::missing_arguments("Reshuffler needs a base 'seed' value.");

        if (conf.reshuffler_opts.block_size == 0)
            throw valid_arguments::exceptions::invalid_setup("Reshuffler", "Block size must be non-zero positive value.");

        if (conf.reshuffler_opts.chunk_size == 0)
            throw valid_arguments::exceptions::invalid_setup("Reshuffler", "Chunk size must be non-zero positive value.");

        if (conf.reshuffler_opts.type == "bit") {

            if (!valid_arguments::reshuffler_bit.count(conf.reshuffler_opts.algorithm)) {
                throw valid_arguments::exceptions::app_invalid_argument("Unknown reshuffler-bit algorithm: " + conf.reshuffler_opts.algorithm);
            }
            
        } else if (conf.reshuffler_opts.type == "chunk") {

            if (!valid_arguments::reshuffler_chunk.count(conf.reshuffler_opts.algorithm)) {
                throw valid_arguments::exceptions::app_invalid_argument("Unknown reshuffler-chunk algorithm: " + conf.reshuffler_opts.algorithm);
            }

        } else {
            throw valid_arguments::exceptions::app_invalid_argument("Unknown reshuffler type set: " + conf.reshuffler_opts.type);
        }

        // All settings validated.
        // only provide warnings if the setting is enabled.

        if (conf.warnings) {

            if (conf.memory_usage < locker_limits_minimum_ram_bytes) {
                throw valid_arguments::exceptions::app_invalid_argument("It is recommended to allow at least 2MB of ram usage to avoid performance issues. Pass --app-warnings-off to turn off warnings.");
            }

        }

        if (conf.shuffler_opts.warnings) {

            if (*conf.shuffler_opts.modification_factor == 1)
                throw valid_arguments::exceptions::invalid_setup("Shuffler", "Setting modification factor as one is highly insecure. Pass --no-warnings sub-option to force the input.");

        }

        // all good.
        return;

    };

};

#ifdef LOCKER_APP_CONFIG_PRINTER_REQUIRED

#include <iostream>


// Helper for optional
template<typename T>
inline std::ostream& print_optional(std::ostream& os, const std::optional<T>& opt) {
    if (opt.has_value()) os << opt.value();
    else                 os << "(none)";
    return os;
}

// Helper for vector
template<typename T>
inline std::ostream& print_vector(std::ostream& os, const std::vector<T>& vec) {
    os << "[";
    for (std::size_t i = 0; i < vec.size(); ++i) {
        os << vec[i];
        if (i + 1 < vec.size()) os << ", ";
    }
    os << "]";
    return os;
}

inline std::ostream& operator<<(std::ostream& os, const shuffler_options& s) {
    os << "  shuffler_options:\n"
       << "    algorithm         : " << s.algorithm   << "\n"
       << "    chunk_size        : " << s.chunk_size   << "\n"
       << "    modification_factor: ";
    print_optional(os, s.modification_factor);
    return os << "\n";
}

inline std::ostream& operator<<(std::ostream& os, const reshuffler_options& r) {
    os << "  reshuffler_options:\n"
       << "    algorithm  : " << r.algorithm  << "\n"
       << "    type       : " << r.type        << "\n"
       << "    chunk_size : " << r.chunk_size  << "\n"
       << "    block_size : " << r.block_size  << "\n"
       << "    seed       : "; print_optional(os, r.seed);
    os << "\n"
       << "    extargs    : "; print_vector(os, r.extargs);
    return os << "\n";
}

inline std::ostream& operator<<(std::ostream& os, const app_config& c) {
    os << "app_config:\n"
       << "  input_file  : " << c.input_file            << "\n"
       << "  output_file : " << c.output_file           << "\n"
       << "  is_encoder  : " << std::boolalpha << c.is_encoder << "\n"
       << c.shuffler_opts
       << c.reshuffler_opts;
    return os;
}

#endif

#endif