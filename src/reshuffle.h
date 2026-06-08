#pragma once
#ifndef LOCKER_RESHUFFLE_H
#define LOCKER_RESHUFFLE_H

#include <stddef.h>
#ifndef SIZE_MAX
#define SIZE_MAX 0xffffffffffffffffULL
#endif

#include <vector>

namespace reshuffle {

    struct reshuffle_context_t {

        void* buf;
        size_t chunk_size_bits;
        size_t block_size;
        size_t seed;

        std::vector<unsigned long> extargs;

        std::size_t offset_from_previous_block;
        // optional to validate
        // size_t max_size_bits;

    };

    namespace chunk {

        namespace algorithms {
            void mirror(const struct reshuffle_context_t& context);
        };

    };

    namespace bit {

        namespace algorithms {
            void mirror(const struct reshuffle_context_t& context);
        }
        
    };

};

#endif