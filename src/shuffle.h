#pragma once
#ifndef SHUFFLE_H
#define SHUFFLE_H

#include <stddef.h>
#ifndef SIZE_MAX
#define SIZE_MAX 0xffffffffffffffffULL
#endif

namespace shuffle {

    // shuffle_not works as both encoder and decoder
    void shuffle_not(void* ptr, size_t size_bits, size_t chunk_size_bits, unsigned short modification_factor);
    void shuffle_state_machine_E(void* ptr, size_t size_bits, size_t chunk_size_bits, unsigned short modification_factor);
    void shuffle_state_machine_D(void* ptr, size_t size_bits, size_t chunk_size_bits, unsigned short modification_factor);
    void shuffle_prefix_parity_E(void* ptr, size_t size_bits, size_t chunk_size_bits, unsigned short modification_factor);
    void shuffle_prefix_parity_D(void* ptr, size_t size_bits, size_t chunk_size_bits, unsigned short modification_factor);

};

namespace reshuffle {

    namespace chunk {

        void mirror(void* buf, size_t max_size_bits, size_t chunk_size_bits, size_t chunks_in_reshuffle_block, size_t position, bool direction);

    };

    namespace bit {
        void mirror(void* buf, size_t max_size_bits, size_t chunk_size_bits, size_t chunks_in_reshuffle_block, size_t position, bool direction);
    };

};

#endif