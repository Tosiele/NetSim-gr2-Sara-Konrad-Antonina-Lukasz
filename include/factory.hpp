#ifndef FACTORY_HPP
#define FACTORY_HPP
#include <map>
#include <string>
#include <iostream>
#include <sstream>
#include <vector>

#include "types.hpp"
#include "nodes.hpp"
#include "helpers.hpp"

// Forward declarations
class Ramp;
class Worker;
class Storehouse;


class Factory{
    private:
        template<class Node>
        void remove_receiver(NodeCollection<Node>& collection, ElementID id);

    
    public:

    void do_deliveries(Time);
    void do_package_passing();
    void do_work(Time);
};

//function that makes a Factory based on the config file
Factory load_factory_structure(std::istream &is);

//helper functions for save_factory_structure
std::string receiver_save_func(auto &rp);
void ramp_save_func(Ramp &ramp, std::ostream &os);
void worker_save_func(Worker &worker, std::ostream &os);
void storehouse_save_func(Storehouse &storehouse, std::ostream &os);

//function that makes a factory structure report
void save_factory_structure(Factory &factory, std::ostream &os);

#endif //FACTORY_HPP