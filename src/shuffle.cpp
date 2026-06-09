// Copyright (C) 2026 Csral chaturyasral@gmail.com
// SPDX-License-Identifier: GPL-3.0

#include "shuffle.h"
#include <vector>
#include <iostream>
#include <stdio.h>

void shuffle::algorithms::shuffle_not(const struct shuffle_context_t& context) {

    if (context.chunk_size_bits == 0)
        return;

    if (context.modification_factor == 0)
        throw std::runtime_error("Invalid modification factor!");

    size_t bits_processed = 0;
    unsigned char* byte_ptr = (unsigned char*) context.ptr;
    
    while (bits_processed < context.size_bits) {
        
        size_t i = 0;

        while (i < context.chunk_size_bits) {

            if (bits_processed + i >= context.size_bits) break;

            if ((i + 1) % context.modification_factor == 0) {

                size_t current_bit = bits_processed + i;
                size_t current_bytes = (current_bit / 8);
                current_bit = current_bit % 8;

                *(byte_ptr + current_bytes) ^= ((1UL) << (7 - current_bit));

            }

            i++;

        }

        bits_processed += i;

    }

}

void shuffle::algorithms::state_machine_E(const struct shuffle_context_t& context) {

    if (context.chunk_size_bits == 0)
        return;

    // Redundant
    if (context.modification_factor == 0)
        throw std::runtime_error("Invalid modification factor!");

    size_t bits_processed = 0;
    unsigned char state = 0;
    unsigned char* byte_ptr = (unsigned char*) context.ptr;
    
    while (bits_processed < context.size_bits) {
        
        size_t i = 0;

        while (i < context.chunk_size_bits) {

            if (bits_processed + i >= context.size_bits) break;

            if ((i + 1) % context.modification_factor == 0) {
            
            
                size_t current_bit = bits_processed + i;
                size_t current_bytes = (current_bit / 8);
                current_bit = current_bit % 8;

                unsigned char mask = state & 0x01;
                state = ((state << 1) & 0xFF) | (( *(byte_ptr + current_bytes) >> (7 - current_bit)) & 0x01);
                *(byte_ptr + current_bytes) ^= ((mask) << (7 - current_bit));

            }

            i++;

        }

        bits_processed += i;

    }

}

void shuffle::algorithms::state_machine_D(const struct shuffle_context_t& context) {

    if (context.chunk_size_bits == 0)
        return;

    if (context.modification_factor == 0)
        throw std::runtime_error("Invalid modification factor!");

    size_t bits_processed = 0;
    unsigned char state = 0;
    unsigned char* byte_ptr = (unsigned char*) context.ptr;
    
    while (bits_processed < context.size_bits) {
        
        size_t i = 0;

        while (i < context.chunk_size_bits) {

            if (bits_processed + i >= context.size_bits) break;

            if ((i + 1) % context.modification_factor == 0) {
            
            
                size_t current_bit = bits_processed + i;
                size_t current_bytes = (current_bit / 8);
                current_bit = current_bit % 8;

                unsigned char mask = state & 0x01;
                *(byte_ptr + current_bytes) ^= ((mask) << (7 - current_bit));
                state = ((state << 1) & 0xFF) | (( *(byte_ptr + current_bytes) >> (7 - current_bit)) & 0x01);

            }

            i++;

        }

        bits_processed += i;

    }

}

void shuffle::algorithms::prefix_parity_E(const struct shuffle_context_t& context) {

    if (context.chunk_size_bits == 0)
        return;

    if (context.modification_factor == 0)
        throw std::runtime_error("Invalid modification factor!");

    size_t bits_processed = 0;
    size_t num_ones = 0;
    unsigned char* byte_ptr = (unsigned char*) context.ptr;
    
    while (bits_processed < context.size_bits) {
        
        size_t i = 0;

        while (i < context.chunk_size_bits) {

            if (bits_processed + i >= context.size_bits) break;

            size_t current_bit = bits_processed + i;
            size_t current_bytes = (current_bit / 8);
            current_bit = current_bit % 8;

            bool original_bit_status = (((*(byte_ptr + current_bytes)) >> (7 - current_bit)) & 0x01);

            if ((i + 1) % context.modification_factor == 0) {        
                unsigned char parity = num_ones % 2;
                *(byte_ptr + current_bytes) ^= ((parity) << (7 - current_bit));
            }

            // update one count at the end
            if (original_bit_status) num_ones++;
            i++;

        }

        bits_processed += i;

    }

}

void shuffle::algorithms::prefix_parity_D(const struct shuffle_context_t& context) {
 
    if (context.chunk_size_bits == 0)
        return;

    if (context.modification_factor == 0)
        throw std::runtime_error("Invalid modification factor!");

    size_t bits_processed = 0;
    size_t num_ones = 0;
    unsigned char* byte_ptr = (unsigned char*) context.ptr;
    
    while (bits_processed < context.size_bits) {
        
        size_t i = 0;

        while (i < context.chunk_size_bits) {

            if (bits_processed + i >= context.size_bits) break;

            size_t current_bit = bits_processed + i;
            size_t current_bytes = (current_bit / 8);
            current_bit = current_bit % 8;

            bool original_bit_status = (((*(byte_ptr + current_bytes)) >> (7 - current_bit)) & 0x01);

            if ((i + 1) % context.modification_factor == 0) {        
                unsigned char parity = num_ones % 2;
                *(byte_ptr + current_bytes) ^= ((parity) << (7 - current_bit));
                original_bit_status = (((*(byte_ptr + current_bytes)) >> (7 - current_bit)) & 0x01);
            }

            // update one count at the end
            if (original_bit_status) num_ones++;
            i++;

        }

        bits_processed += i;

    }

}