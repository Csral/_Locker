#pragma once
#ifndef ARGUMENTS_REGISTRY_H
#define ARGUMENTS_REGISTRY_H

#include <string>
#include <vector>
#include <optional>
#include <iostream>

struct argument_registry_entry {

    std::string name;
    std::vector<std::string> aliases;
    std::string description;

};

typedef struct argument_registry_entry argument_registry_t;
typedef std::vector<argument_registry_t> registry_t;

namespace arguments_registry {

    inline const std::vector<argument_registry_t> shuffler = {

        {
            "not",
            {},
            "Flips the bit at every modification factor position"
        },

        {
            "state_machine",
            { "sm" },
            "Modifies bit at every modification factor position based on a global state"
        },

        {
            "prefix_parity",
            { "pp" },
            "Modifies bit at every modification factor position based on the bits before it"
        }

    };

    inline const std::vector<argument_registry_t> reshuffler_bit = {
        
        {
            "mirror",
            {},
            "Places mirror at specified position (seed) and flips all bits before it"
        }

    };

    inline const std::vector<argument_registry_t> reshuffler_chunk = {
        
    };

    inline const std::vector<argument_registry_t> reshuffler_type = {
        {
            "bit",
            {},
            "Perform reshuffle bit-wise in which the bits within chunks are not secured and are shuffled throughout the Shuffle Block."
        },

        {
            "chunk",
            {},
            "Perform reshuffle chunk-wise in which the bits within a chunk are not shuffled."
        }
    };

    inline void print_registry(const registry_t& registry) {
        if (registry.empty()) {
            std::cout << "    (none)\n";
            return;
        }

        for (const auto& entry : registry) {
            std::cout << "    " << entry.name;

            if (!entry.aliases.empty()) {
                
                std::cout << " (";

                for (std::size_t i = 0; i < entry.aliases.size(); ++i) {
                    std::cout << entry.aliases[i];
                    if (i + 1 < entry.aliases.size())
                        std::cout << ", ";
                }

                std::cout << ")";
            }

            std::cout << '\n'
                    << "        "
                    << entry.description
                    << "\n\n";
        }
    
    }

};

#endif