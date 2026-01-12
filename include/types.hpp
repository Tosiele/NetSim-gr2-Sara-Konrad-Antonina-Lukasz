//
// Created by konfe on 07.01.2026.
//

#ifndef TYPES_HPP
#define TYPES_HPP
#include <random>

/// used to represent IDs of elements (packages)
using ElementID = int;
/// represents simulation time
using Time = int;
/// represents time needed to complete tasks (like worker processing)
using TimeOffset = int;


std::random_device rd;
std::mt19937 rng(rd());
double probability_generator() {
    return std::generate_canonical<double, 10>(rng);
}
#endif //TYPES_HPP
