#pragma once
#ifndef U_LOCKER_ARGS_H
#define U_LOCKER_ARGS_H

#include <string>
#include <vector>
#include <optional>
#include <unordered_set>
#include <unordered_map>
#include <stdexcept>

struct shuffler_options {

    std::string algorithm = "not";
    std::size_t chunk_size = 8ULL;
    std::optional<std::size_t> modification_factor;

};

struct reshuffler_options {

    std::string algorithm = "mirror";
    std::string type = "bit";
    std::size_t chunk_size = 8ULL;
    std::size_t block_size = 4ULL;
    std::optional<std::size_t> seed;
    std::vector<unsigned long> extargs;

};

struct app_config {

    std::string input_file;
    std::string output_file;
    bool is_encoder = true;

    struct shuffler_options shuffler_opts;
    struct reshuffler_options reshuffler_opts;

};

namespace valid_arguments {

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

};

#ifdef LOCKER_APP_CONFIG_PRINTER_REQUIRED

#include <iostream>


// Helper for optional
template<typename T>
std::ostream& print_optional(std::ostream& os, const std::optional<T>& opt) {
    if (opt.has_value()) os << opt.value();
    else                 os << "(none)";
    return os;
}

// Helper for vector
template<typename T>
std::ostream& print_vector(std::ostream& os, const std::vector<T>& vec) {
    os << "[";
    for (std::size_t i = 0; i < vec.size(); ++i) {
        os << vec[i];
        if (i + 1 < vec.size()) os << ", ";
    }
    os << "]";
    return os;
}

std::ostream& operator<<(std::ostream& os, const shuffler_options& s) {
    os << "  shuffler_options:\n"
       << "    algorithm         : " << s.algorithm   << "\n"
       << "    chunk_size        : " << s.chunk_size   << "\n"
       << "    modification_factor: ";
    print_optional(os, s.modification_factor);
    return os << "\n";
}

std::ostream& operator<<(std::ostream& os, const reshuffler_options& r) {
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

std::ostream& operator<<(std::ostream& os, const app_config& c) {
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