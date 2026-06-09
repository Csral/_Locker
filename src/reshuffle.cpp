// Copyright (C) 2026 Csral chaturyasral@gmail.com
// SPDX-License-Identifier: GPL-3.0

#include "reshuffle.h"
#include <stdexcept>

void reshuffle::bit::algorithms::mirror(const struct reshuffle_context_t& context) {

    // Should only get one reshuffle block.

    size_t position = context.seed;
    bool direction = false;

    if (context.extargs.size() > 0)
        direction = (context.extargs.at(0) != 0);

    if (context.chunk_size_bits == 0)
        return;

    if (position == 0 && (!direction)) return; // nothing to do with mirror.

    unsigned char* byte_buf = (unsigned char*) context.buf;
    size_t num_bits_read = 0;
    size_t bits_per_reshuffle_block = context.block_size * context.chunk_size_bits;
    
    // Position - after which chunk to place the mirror.
    // So the raw bit position is (chunk_size_bits * position)

    size_t position_in_bits = position * context.chunk_size_bits;
    if (position_in_bits > bits_per_reshuffle_block)
        throw std::overflow_error("Mirror position exceeds maximum workspace.");

    unsigned char _tmp = 0x00U;
    size_t current_bit = 0x00U;
    size_t current_bytes = 0x00U;

    size_t mirror_bytes = 0x00U;
    size_t mirror_bit = 0x00U;

    if (direction) {
        // Inverse nature
        // mirror to right
        while (num_bits_read + position_in_bits < (bits_per_reshuffle_block / 2)) {

            current_bytes = (context.offset_from_previous_block + position_in_bits + num_bits_read) / 8;
            current_bit = (context.offset_from_previous_block + position_in_bits +  num_bits_read) % 8;

            mirror_bytes = (context.offset_from_previous_block + bits_per_reshuffle_block - 1 - num_bits_read) / 8;
            mirror_bit = (context.offset_from_previous_block + bits_per_reshuffle_block - 1 - num_bits_read) % 8;

            _tmp = ((*(byte_buf + current_bytes) >> (7 - current_bit)) & 0x01); // get current bit
            *(byte_buf + current_bytes) &= (~((1U) << (7 - current_bit)));
            *(byte_buf + current_bytes) |= (((*(byte_buf + mirror_bytes) >> (7 - mirror_bit)) & 0x01) << (7 - current_bit));
            *(byte_buf + mirror_bytes) &= (~((1U) << (7 - mirror_bit)));
            *(byte_buf + mirror_bytes) |= (_tmp << (7 - mirror_bit));

            num_bits_read++;

        }

    } else {
        // Default configuration
        // mirror to left
        while (num_bits_read < (position_in_bits / 2)) {

            current_bytes = (num_bits_read + context.offset_from_previous_block) / 8;
            current_bit = (context.offset_from_previous_block + num_bits_read) % 8;

            mirror_bytes = (context.offset_from_previous_block + position_in_bits - 1 - num_bits_read) / 8;
            mirror_bit = (context.offset_from_previous_block + position_in_bits - 1 - num_bits_read) % 8;

            _tmp = ((*(byte_buf + current_bytes) >> (7 - current_bit)) & 0x01); // get current bit
            *(byte_buf + current_bytes) &= (~((1U) << (7 - current_bit)));
            *(byte_buf + current_bytes) |= (((*(byte_buf + mirror_bytes) >> (7 - mirror_bit)) & 0x01) << (7 - current_bit));
            *(byte_buf + mirror_bytes) &= (~((1U) << (7 - mirror_bit)));
            *(byte_buf + mirror_bytes) |= (_tmp << (7 - mirror_bit));

            num_bits_read++;

        }

    }

}

// void reshuffle::chunk::algorithms::mirror(const struct reshuffle_context_t& context) {

//     // Should only get one reshuffle block.

//     size_t position = context.seed;
//     bool direction = false;

//     if (context.extargs.size() > 0)
//         direction = (context.extargs.at(0) != 0);

//     if (context.chunk_size_bits == 0)
//         return;

//     if (position == 0 && (!direction)) return; // nothing to do with mirror.

//     unsigned char* byte_buf = (unsigned char*) context.buf;
//     size_t num_bits_read = 0;
//     size_t num_chunks_read = 0;
//     size_t bits_per_reshuffle_block = context.block_size * context.chunk_size_bits;
    
//     // Position - after which chunk to place the mirror.
//     // So the raw bit position is (chunk_size_bits * position)

//     size_t position_in_bits = position * context.chunk_size_bits;
//     if (position_in_bits > bits_per_reshuffle_block)
//         throw std::overflow_error("Mirror position exceeds maximum workspace.");

//     unsigned char _tmp = 0x00U;
//     size_t current_bit = 0x00U;
//     size_t current_bytes = 0x00U;

//     size_t mirror_chunk = 0x00U;
//     size_t mirror_bytes = 0x00U;
//     size_t mirror_bit = 0x00U;

//     if (direction) {
//         // Inverse nature
//         // mirror to right
        
//         while (num_bits_read + position_in_bits < bits_per_reshuffle_block) {

//             current_bytes = (position_in_bits + num_bits_read) / 8;
//             current_bit = (position_in_bits +  num_bits_read) % 8;

//             mirror_bytes = ((position_in_bits / 2) + 1 + num_bits_read) / 8;
//             mirror_bit = ((position_in_bits / 2) + 1 + num_bits_read) % 8;

//             _tmp = ((*(byte_buf + current_bytes) >> (7 - current_bit)) & 0x01); // get current bit
//             *(byte_buf + current_bytes) &= (~((1U) << (7 - current_bit)));
//             *(byte_buf + current_bytes) |= (((*(byte_buf + mirror_bytes) >> (7 - mirror_bit)) & 0x01) << (7 - current_bit));
//             *(byte_buf + mirror_bytes) &= (~((1U) << (7 - mirror_bit)));
//             *(byte_buf + mirror_bytes) |= (_tmp << (7 - mirror_bit));

//             num_bits_read++;

//         }

//     } else {
//         // Default configuration
//         // mirror to left
        
//         while (num_chunks_read < (position / 2)) {

//             size_t i = 0;

//             while (i < context.chunk_size_bits) {

//                 current_bytes = ((num_chunks_read * context.chunk_size_bits) + i) / 8;
//                 current_bit = ((num_chunks_read * context.chunk_size_bits) + i) % 8;

//                 mirror_bit = (position_in_bits - 1 - (context.chunk_size_bits * (num_chunks_read + 1)) + i);

//                 mirror_bytes = mirror_bit / 8;
//                 mirror_bit = (mirror_bit - 1) % 8;

//                 _tmp = ((*(byte_buf + current_bytes) >> (7 - current_bit)) & 0x01); // get current bit
//                 *(byte_buf + current_bytes) &= (~((1U) << (7 - current_bit)));
//                 *(byte_buf + current_bytes) |= (((*(byte_buf + mirror_bytes) >> (7 - mirror_bit)) & 0x01) << (7 - current_bit));
//                 *(byte_buf + mirror_bytes) &= (~((1U) << (7 - mirror_bit)));
//                 *(byte_buf + mirror_bytes) |= (_tmp << (7 - mirror_bit));

//                 i++;

//             }

//             num_chunks_read++;

//         }

//     }

// }