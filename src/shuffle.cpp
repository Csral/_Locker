#include "shuffle.h"
#include <vector>
#include <iostream>
#include <stdio.h>

void shuffle::shuffle_not(void* ptr, size_t size_bits, size_t chunk_size_bits, unsigned short modification_factor) {

    if (chunk_size_bits == 0)
        return;

    if (modification_factor == 0)
        throw "Invalid modification factor!";

    size_t bits_processed = 0;
    unsigned char* byte_ptr = (unsigned char*) ptr;
    
    while (bits_processed < size_bits) {
        
        size_t i = 0;

        while (i < chunk_size_bits) {

            if (bits_processed + i >= size_bits) break;

            if ((i + 1) % modification_factor == 0) {

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

void shuffle::shuffle_state_machine_E(void* ptr, size_t size_bits, size_t chunk_size_bits, unsigned short modification_factor) {

    if (chunk_size_bits == 0)
        return;

    if (modification_factor == 0)
        throw "Invalid modification factor!";

    size_t bits_processed = 0;
    unsigned char state = 0;
    unsigned char* byte_ptr = (unsigned char*) ptr;
    
    while (bits_processed < size_bits) {
        
        size_t i = 0;

        while (i < chunk_size_bits) {

            if (bits_processed + i >= size_bits) break;

            if ((i + 1) % modification_factor == 0) {
            
            
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

void shuffle::shuffle_state_machine_D(void* ptr, size_t size_bits, size_t chunk_size_bits, unsigned short modification_factor) {

    if (chunk_size_bits == 0)
        return;

    if (modification_factor == 0)
        throw "Invalid modification factor!";

    size_t bits_processed = 0;
    unsigned char state = 0;
    unsigned char* byte_ptr = (unsigned char*) ptr;
    
    while (bits_processed < size_bits) {
        
        size_t i = 0;

        while (i < chunk_size_bits) {

            if (bits_processed + i >= size_bits) break;

            if ((i + 1) % modification_factor == 0) {
            
            
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

void shuffle::shuffle_prefix_parity_E(void* ptr, size_t size_bits, size_t chunk_size_bits, unsigned short modification_factor) {

    if (chunk_size_bits == 0)
        return;

    if (modification_factor == 0)
        throw "Invalid modification factor!";

    size_t bits_processed = 0;
    size_t num_ones = 0;
    unsigned char* byte_ptr = (unsigned char*) ptr;
    
    while (bits_processed < size_bits) {
        
        size_t i = 0;

        while (i < chunk_size_bits) {

            if (bits_processed + i >= size_bits) break;

            size_t current_bit = bits_processed + i;
            size_t current_bytes = (current_bit / 8);
            current_bit = current_bit % 8;

            bool original_bit_status = (((*(byte_ptr + current_bytes)) >> (7 - current_bit)) & 0x01);

            if ((i + 1) % modification_factor == 0) {        
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

void shuffle::shuffle_prefix_parity_D(void* ptr, size_t size_bits, size_t chunk_size_bits, unsigned short modification_factor) {
 
    if (chunk_size_bits == 0)
        return;

    if (modification_factor == 0)
        throw "Invalid modification factor!";

    size_t bits_processed = 0;
    size_t num_ones = 0;
    unsigned char* byte_ptr = (unsigned char*) ptr;
    
    while (bits_processed < size_bits) {
        
        size_t i = 0;

        while (i < chunk_size_bits) {

            if (bits_processed + i >= size_bits) break;

            size_t current_bit = bits_processed + i;
            size_t current_bytes = (current_bit / 8);
            current_bit = current_bit % 8;

            bool original_bit_status = (((*(byte_ptr + current_bytes)) >> (7 - current_bit)) & 0x01);

            if ((i + 1) % modification_factor == 0) {        
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

void reshuffle::bit::mirror(void* buf, size_t max_size_bits, size_t chunk_size_bits, size_t chunks_in_reshuffle_block, size_t position, bool direction = false) {

    // Should only get one reshuffle block.

    if (chunk_size_bits == 0)
        return;

    if (max_size_bits == 0)
        return;

    if (position == 0 && (!direction)) return; // nothing to do with mirror.

    unsigned char* byte_buf = (unsigned char*) buf;
    size_t num_bits_read = 0;
    size_t bits_per_reshuffle_block = chunks_in_reshuffle_block * chunk_size_bits;

    if (bits_per_reshuffle_block != max_size_bits)
        throw std::range_error("Bits per reshuffle block and max operable range of bits are not same.");
    
    // Position - after which chunk to place the mirror.
    // So the raw bit position is (chunk_size_bits * position)

    size_t position_in_bits = position * chunk_size_bits;
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

            current_bytes = (position_in_bits + num_bits_read) / 8;
            current_bit = (position_in_bits +  num_bits_read) % 8;

            mirror_bytes = (bits_per_reshuffle_block - 1 - num_bits_read) / 8;
            mirror_bit = (bits_per_reshuffle_block - 1 - num_bits_read) % 8;

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

            current_bytes = num_bits_read / 8;
            current_bit = num_bits_read % 8;

            mirror_bytes = (position_in_bits - 1 - num_bits_read) / 8;
            mirror_bit = (position_in_bits - 1 - num_bits_read) % 8;

            _tmp = ((*(byte_buf + current_bytes) >> (7 - current_bit)) & 0x01); // get current bit
            *(byte_buf + current_bytes) &= (~((1U) << (7 - current_bit)));
            *(byte_buf + current_bytes) |= (((*(byte_buf + mirror_bytes) >> (7 - mirror_bit)) & 0x01) << (7 - current_bit));
            *(byte_buf + mirror_bytes) &= (~((1U) << (7 - mirror_bit)));
            *(byte_buf + mirror_bytes) |= (_tmp << (7 - mirror_bit));

            num_bits_read++;

        }

    }

}

void reshuffle::chunk::mirror(void* buf, size_t max_size_bits, size_t chunk_size_bits, size_t chunks_in_reshuffle_block, size_t position, bool direction = false) {

    // Should only get one reshuffle block.

    if (chunk_size_bits == 0)
        return;

    if (max_size_bits == 0)
        return;

    if (position == 0 && (!direction)) return; // nothing to do with mirror.

    unsigned char* byte_buf = (unsigned char*) buf;
    size_t num_bits_read = 0;
    size_t num_chunks_read = 0;
    size_t bits_per_reshuffle_block = chunks_in_reshuffle_block * chunk_size_bits;

    if (bits_per_reshuffle_block != max_size_bits)
        throw std::range_error("Bits per reshuffle block and max operable range of bits are not same.");
    
    // Position - after which chunk to place the mirror.
    // So the raw bit position is (chunk_size_bits * position)

    size_t position_in_bits = position * chunk_size_bits;
    if (position_in_bits > bits_per_reshuffle_block)
        throw std::overflow_error("Mirror position exceeds maximum workspace.");

    unsigned char _tmp = 0x00U;
    size_t current_bit = 0x00U;
    size_t current_bytes = 0x00U;

    size_t mirror_chunk = 0x00U;
    size_t mirror_bytes = 0x00U;
    size_t mirror_bit = 0x00U;

    if (direction) {
        // Inverse nature
        // mirror to right
        
        while (num_bits_read + position_in_bits < bits_per_reshuffle_block) {

            current_bytes = (position_in_bits + num_bits_read) / 8;
            current_bit = (position_in_bits +  num_bits_read) % 8;

            mirror_bytes = ((position_in_bits / 2) + 1 + num_bits_read) / 8;
            mirror_bit = ((position_in_bits / 2) + 1 + num_bits_read) % 8;

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
        
        while (num_chunks_read < (position / 2)) {

            size_t i = 0;

            while (i < chunk_size_bits) {

                current_bytes = ((num_chunks_read * chunk_size_bits) + i) / 8;
                current_bit = ((num_chunks_read * chunk_size_bits) + i) % 8;

                mirror_bit = (position_in_bits - 1 - (chunk_size_bits * (num_chunks_read + 1)) + i);

                mirror_bytes = mirror_bit / 8;
                mirror_bit = (mirror_bit - 1) % 8;

                std::cout << "Mirrror bit: " << mirror_bit << "\n";

                _tmp = ((*(byte_buf + current_bytes) >> (7 - current_bit)) & 0x01); // get current bit
                *(byte_buf + current_bytes) &= (~((1U) << (7 - current_bit)));
                *(byte_buf + current_bytes) |= (((*(byte_buf + mirror_bytes) >> (7 - mirror_bit)) & 0x01) << (7 - current_bit));
                *(byte_buf + mirror_bytes) &= (~((1U) << (7 - mirror_bit)));
                *(byte_buf + mirror_bytes) |= (_tmp << (7 - mirror_bit));

                i++;

            }

            num_chunks_read++;

        }

    }

}

int main(void) {

    std::vector<unsigned char> ptr = {
        0x80,
        0x00,
        0x12,
        0x90,
        0x50
    };

    std::cout << "Printing: \n";
    
    for (auto x : ptr) {
        printf("%x\n", x);
    }
    
    reshuffle::bit::mirror((void*) ptr.data(), 14, 7, 2, 2);

    // shuffle::shuffle_prefix_parity_E((void*) ptr.data(), 13, 13, 3);
    
    std::cout << "Printing encoded: \n";
    
    for (auto x : ptr) {
        printf("%x\n", x);
    }

    // shuffle::shuffle_prefix_parity_D((void*) ptr.data(), 13, 13, 3);

    reshuffle::bit::mirror((void*) ptr.data(), 14, 7, 2, 2);
    
    std::cout << "Printing decoded: \n";
    
    for (auto x : ptr) {
        printf("%x\n", x);
    }

    return 0;

}