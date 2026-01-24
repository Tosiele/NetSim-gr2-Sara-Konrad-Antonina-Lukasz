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
#endif //NETSIM_GR2_SARA_KONRAD_ANTONINA_HELPERS_HPP