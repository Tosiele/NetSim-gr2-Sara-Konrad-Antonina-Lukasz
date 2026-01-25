//
// Created by konfe on 24.01.2026.
//

#ifndef NETSIM_GR2_SARA_KONRAD_ANTONINA_HELPERS_HPP
#define NETSIM_GR2_SARA_KONRAD_ANTONINA_HELPERS_HPP
#include <map>
#include <string>
#include <iostream>
#include <sstream>
#include <vector>

#include "nodes.hpp"

enum class ElementType { //or regular enum?
    RAMP,
    WORKER,
    STOREHOUSE,
    LINK
};
struct ParsedLineData {
    ElementType element_type;
    std::map<std::string, std::string> parameters;
    //only for tests
    bool operator==(const ParsedLineData &other) const {return (element_type == other.element_type && parameters == other.parameters);}
};


ParsedLineData parse_line(const std::string &line);

Factory load_factory_structure(std::istream &is);

//helper functions for save_factory_structure
std::ostream& receiver_save_func(ReceiverPreferences rp);
void ramp_save_func(Ramp &ramp, std::ostream &os);
void worker_save_func(Worker &worker, std::ostream &os);
void storehouse_save_func(Storehouse &storehouse, std::ostream &os);

void save_factory_structure(Factory &factory, std::ostream &os);

#endif //NETSIM_GR2_SARA_KONRAD_ANTONINA_HELPERS_HPP