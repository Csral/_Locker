#pragma once
#ifndef SHUFFLE_H
#define SHUFFLE_H

#include <stddef.h>
#ifndef SIZE_MAX
#define SIZE_MAX 0xffffffffffffffffULL
#endif

namespace shuffle {

    struct shuffle_context_t {

        void* ptr;
        size_t size_bits;
        size_t chunk_size_bits;
        size_t modification_factor;

    };

    // shuffle_not works as both encoder and decoder

    namespace algorithms {

        void shuffle_not(const struct shuffle_context_t& context);
        void state_machine_E(const struct shuffle_context_t& context);
        void state_machine_D(const struct shuffle_context_t& context);
        void prefix_parity_E(const struct shuffle_context_t& context);
        void prefix_parity_D(const struct shuffle_context_t& context);
        
    }

};

#endif