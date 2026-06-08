#pragma once
#ifndef ALGORITHM_REGISTRY_H
#define ALGORITHM_REGISTRY_H

#include <functional>
#include <unordered_map>
#include <string>
#include <stdexcept>

#include "shuffle.h"
#include "reshuffle.h"

namespace shuffle {

    using dispatcher = std::function<void(const shuffle::shuffle_context_t&)>;

    namespace registry {

        struct algorithm_registry_entry_t {

            dispatcher encoder;
            dispatcher decoder;

        };

        const std::unordered_map<std::string, algorithm_registry_entry_t> algorithm_mapping {
            
            { "not", { shuffle::algorithms::shuffle_not, shuffle::algorithms::shuffle_not } },
            { "state_machine", { shuffle::algorithms::state_machine_E, shuffle::algorithms::state_machine_D } },
            { "prefix_parity", { shuffle::algorithms::prefix_parity_E, shuffle::algorithms::prefix_parity_D } }
            
        };

    };

    inline void dispatch_algorithm(const std::string& name, bool is_encoder, const shuffle::shuffle_context_t& context) {

        auto it = shuffle::registry::algorithm_mapping.find(name);

        if (it == shuffle::registry::algorithm_mapping.end())
            throw std::runtime_error("Unknown shuffler algorithm: " + name);

        if (is_encoder)
            it->second.encoder(context);
        else
            it->second.decoder(context);

        return;

    };

};

namespace reshuffle {

    using dispatcher = std::function<void(const reshuffle::reshuffle_context_t&)>;

    namespace registry {

        const std::unordered_map<std::string, std::unordered_map<std::string, dispatcher>> algorithm_mapping {
          
            {
                "bit", {
                    { "mirror", reshuffle::bit::algorithms::mirror }
                }
            },

            {
                "chunk", {
                    // { "mirror", reshuffle::chunk::algorithms::mirror }
                }
            }

        };

    };

    inline void dispatch_algorithm(const std::string& type, const std::string& name, const reshuffle::reshuffle_context_t& context) {

        auto registry_it = reshuffle::registry::algorithm_mapping.find(type);

        if (registry_it == reshuffle::registry::algorithm_mapping.end())
            throw std::runtime_error("Unknown reshuffle type: " + type);

        auto it = registry_it->second.find(name);

        if (it == registry_it->second.end())
            throw std::runtime_error("Unknown reshuffle algorithm: " + name);

        it->second(context);

        return;

    };

};

#endif