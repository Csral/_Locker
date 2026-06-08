#pragma once
#ifndef LOCKER_H
#define LOCKER_H

#include <string>
#include <vector>
#include <filesystem>
#include <fstream>

#include "args.h"

class Locker {
    
    private:
    std::string input_file;
    std::string output_file;
    std::ifstream input_file_handle;
    std::ofstream output_file_handler;

    std::size_t ram_usage_allowed = 512*1024*1024ULL;

    // encoder/decoder status
    bool is_encoding = true;

    // Keys.
    size_t shuffle_chunk_size; // chunk size
    size_t modification_factor; // per entry number
    std::string shuffle_algorithm; // Which algorithm to use for shuffling.
    
    /* Reshuffle */
    // chunks_size - How many bits make up a single chunk
    // block_size - How many pairs of chunks make up a single reshuffle block
    // seed - How to reshuffle
    // algorithm - Which algorithm to use for reshuffling.
    // type - bit shuffle or chunk shuffle

    std::string reshuffle_algorithm;
    size_t reshuffle_chunks_size;
    size_t block_size;
    size_t seed;

    // You can reshuffle either bit-wise or chunk-wise.
    // so unless more options are used - a bool suffices this.
    bool type_is_bit = true;

    std::vector<unsigned long> extargs; // Extra arguments. May or may not be used.

    /* Functions */
    void validate_configuration(const struct app_config& configuration);

    public:
    Locker(const struct app_config& configuration);
    ~Locker(void);

    void lock(void);
    
};

#endif