#ifndef NETSIM_GR2_SARA_KONRAD_ANTONINA_HELPERS_HPP
#define NETSIM_GR2_SARA_KONRAD_ANTONINA_HELPERS_HPP
#include <map>
#include <string>
#include <iostream>
#include <sstream>
#include <vector>

enum class ElementType {
    RAMP,
    WORKER,
    STOREHOUSE,
    LINK
};

struct ParsedLineData {
    //struct that stores the type of element
    //along with all its properties as a map
    ElementType element_type;
    std::map<std::string, std::string> parameters;
    //only for tests
    bool operator==(const ParsedLineData &other) const {return (element_type == other.element_type && parameters == other.parameters);}
};

// function that divides the given line and stores the result as ParsedLineData struct
ParsedLineData parse_line(const std::string &line);
#endif //NETSIM_GR2_SARA_KONRAD_ANTONINA_HELPERS_HPP