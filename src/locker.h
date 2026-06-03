#pragma once
#ifndef LOCKER_H
#define LOCKER_H

#include <string>
#include <vector>

class Locker {
    
    private:
    std::string target_file;
    std::string output_file;
    std::ifstream target_handler();
    std::ofstream file_handler();

    // Keys.
    unsigned long bit_count;
    unsigned long number; // per entry number
    std::string alg; // Which algorithm to use for shuffling.
    
    /* Reshuffle */
    // chunks - How many bits make up a single chunk
    // factor - How many pairs of chunks make up a single reshuffle block
    // seed - How to reshuffle
    // sel - Which algorithm to use for reshuffling.
    // type - bit shuffle or chunk shuffle

    unsigned long reshuffle_factor;
    unsigned long reshuffle_chunks;
    unsigned long reshuffle_seed;
    unsigned char reshuffle_type;
    std::string reshuffle_sel;

    std::vector<unsigned long> reshuffle_extargs; // Extra arguments. May or may not be used.
    
    public:
    Locker(const std::string& target, const std::string& output);
    
};

#endif