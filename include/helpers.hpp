//
// Created by konfe on 29.01.2026.
//

#ifndef NETSIM_HELPERS_HPP
#define NETSIM_HELPERS_HPP

#include <random>
#include <fstream>


//declarations for probability_generator implementation
extern std::random_device rd;
extern std::mt19937 rng;
double probability_generator();

//for section_2_tests
inline double simple_func(){return 0.4;}

//helper function for file comparison for section_4_tests
bool compareFiles(const std::string& p1, const std::string& p2);



#endif //NETSIM_HELPERS_HPP