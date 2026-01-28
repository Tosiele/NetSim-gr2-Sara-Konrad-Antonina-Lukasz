//
// Created by konfe on 24.01.2026.
//


#ifndef FACTORY
#define FACTORY

#include <map>
#include <string>
#include <iostream>
#include <sstream>
#include <vector>

#include "nodes.hpp"

enum class ElementType {
    //enumeration class that stores element types
    //possible to pass in the configuration file
    RAMP,
    WORKER,
    STOREHOUSE,
    LINK
};
struct ParsedLineData {

    // struct that stores the type of element
    // along with all its properties as a map
    ElementType element_type;
    std::map<std::string, std::string> parameters;
    // only for tests
    bool operator==(const ParsedLineData &other) const {return (element_type == other.element_type && parameters == other.parameters);}
};


template<class Node>
class NodeCollection {

    public:
        using container_t = typename std::list<Node>;
        using iterator = typename container_t::iterator;
        using const_iterator = typename container_t::const_iterator;

        iterator begin() { return container.begin(); }
        iterator end() { return container.end(); }
        
        const_iterator begin() const { return container.cbegin(); }
        const_iterator end() const { return container.cend(); }
        
        const_iterator cbegin() const { return container.cbegin(); }
        const_iterator cend() const { return container.cend(); }
    private:

};

class Factory{
    public:


    private:
        template<class Node>
        void remove_receiver(NodeCollection<Node>& collection, element_ID id);



};


// function that divides the given line and stores the result as ParsedLineData struct
ParsedLineData parse_line(const std::string &line);

// function that makes a Factory based on the config file
Factory load_factory_structure(std::istream &is);

// helper functions for save_factory_structure

std::string receiver_save_func(auto &rp);
void ramp_save_func(Ramp &ramp, std::ostream &os);
void worker_save_func(Worker &worker, std::ostream &os);
void storehouse_save_func(Storehouse &storehouse, std::ostream &os);


// function that makes a factory structure report
void save_factory_structure(Factory &factory, std::ostream &os);

#endif // FACTORY
