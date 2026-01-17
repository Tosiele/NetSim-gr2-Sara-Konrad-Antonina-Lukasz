//
// Created by konfe on 07.01.2026.
//

#ifndef TYPES_HPP
#define TYPES_HPP
#include <random>
#include <functional>

/// used to represent IDs of elements (packages)
using ElementID = int;
/// represents simulation time
using Time = int;
/// represents time needed to complete tasks (like worker processing)
using TimeOffset = int;

using ProbabilityGenerator = std::function<double()>;

#endif //TYPES_HPP
