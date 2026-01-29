#ifndef FACTORY
#define FACTORY

#include <map>
#include <string>
#include <iostream>
#include <sstream>
#include <vector>

#include "nodes.hpp"
#include "Ramp.hpp"
#include "Worker.hpp"
#include "Package.hpp"

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


class Factory{
    public:
        // Ramp methods
        void add_ramp(const Ramp&& ramp);
        void remove_ramp(ElementID id);
        NodeCollection<Ramp>::iterator find_ramp_by_id(ElementID id);
        NodeCollection<Ramp>::const_iterator find_ramp_by_id(ElementID id) const;
        NodeCollection<Ramp>::const_iterator ramp_cbegin() const { return Ramps.cbegin(); }
        NodeCollection<Ramp>::const_iterator ramp_cend() const { return Ramps.cend(); }
        // Worker methods
        void add_worker(const Worker&& worker);
        void remove_worker(ElementID id);
        NodeCollection<Worker>::iterator find_worker_by_id(ElementID id);
        NodeCollection<Worker>::const_iterator find_worker_by_id(ElementID id) const;
        NodeCollection<Worker>::const_iterator worker_cbegin() const { return Workers.cbegin(); }
        NodeCollection<Worker>::const_iterator worker_cend() const { return Workers.cend(); }
        // Storehouse methods
        void add_storehouse(const Storehouse&& storehouse);
        void remove_storehouse(ElementID id);
        NodeCollection<Storehouse>::iterator find_storehouse_by_id(ElementID id);
        NodeCollection<Storehouse>::const_iterator find_storehouse_by_id(ElementID id) const;
        NodeCollection<Storehouse>::const_iterator storehouse_cbegin() const { return Storehouses.cbegin(); }
        NodeCollection<Storehouse>::const_iterator storehouse_cend() const { return Storehouses.cend(); }

        // other methods
        bool is_consistent();
        void do_deliveries(Time t);
        void do_package_passing();
        void do_work(Time t);

    private:
        NodeCollection<Ramp> Ramps;
        NodeCollection<Worker> Workers;
        NodeCollection<Storehouse> Storehouses;

        template<class Node>
        void remove_receiver(NodeCollection<Node>& collection, ElementID id);
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
