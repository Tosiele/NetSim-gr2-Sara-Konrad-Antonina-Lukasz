//
// Created by konfe on 29.01.2026.
//

#include "helpers.hpp"
#include <random>
#include <fstream>
#include <sstream>
#include "nodes.hpp"


//implementation of probability generator function
std::random_device rd;
std::mt19937 rng(rd());
double probability_generator() {
    return std::generate_canonical<double, 10>(rng);
}

//helper function for file comparison
bool compareFiles(const std::string& p1, const std::string& p2) {
    std::ifstream f1(p1, std::ifstream::binary|std::ifstream::ate);
    std::ifstream f2(p2, std::ifstream::binary|std::ifstream::ate);

    if (f1.fail() || f2.fail()) {
        return false; //file problem
    }

    if (f1.tellg() != f2.tellg()) {
        return false; //size mismatch
    }

    //seek back to beginning and use std::equal to compare contents
    f1.seekg(0, std::ifstream::beg);
    f2.seekg(0, std::ifstream::beg);
    return std::equal(std::istreambuf_iterator<char>(f1.rdbuf()),
                      std::istreambuf_iterator<char>(),
                      std::istreambuf_iterator<char>(f2.rdbuf()));
}

