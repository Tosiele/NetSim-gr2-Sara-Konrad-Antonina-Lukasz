//
// Created by konfe on 24.01.2026.
//

#include "../include/factory.hpp"
#include "../include/nodes.hpp"
#include "../include/types.hpp"

ParsedLineData parse_line(const std::string &line) {
    /*function that divides the given line and stores the result as ParsedLineData struct
     *takes a std::string address of a line
     *returns struct ParsedLineData
     */
    //defining the empty struct and helpful variables
    ParsedLineData parsed_line;
    std::vector<std::string> tokens;
    std::string token;

    //converting the line to a stream
    std::istringstream token_stream(line);
    char delimiter = ' ';

    //getting the first part of the line
    std::getline(token_stream, token, delimiter);
    std::map<std::string,ElementType> tags {
        {"LOADING_RAMP",ElementType::RAMP},
        {"WORKER",ElementType::WORKER},
        {"STOREHOUSE",ElementType::STOREHOUSE},
        {"LINK",ElementType::LINK}
    };

    //checking the matching element type
    if (tags.find(token) != tags.end()) {
        parsed_line.element_type = tags.at(token);
    }
    //error handling
    else {
        throw std::invalid_argument("No matching tag");
    }

    //getting all the other key-value pairs and storing them in a map
    std::string key;
    std::string value;
    while (std::getline(token_stream, token, delimiter)) {
        std::istringstream keyval_stream(token);
        char delim = '=';
        std::getline(keyval_stream, key, delim);
        std::getline(keyval_stream, value, delimiter);
        parsed_line.parameters.emplace(key, value);
    }
    //returning the complete struct
    return parsed_line;
}

Factory load_factory_structure(std::istream &is) {
    /*function that makes a Factory based on the config file
     *takes the std::istream address (so it's possible to call on both std::cin and std::ifstream)
     *returns the Factory object
     */

    //creating an empty factory
    Factory F{};

    //other helpful variables
    std::string line;
    ElementID id;
    TimeOffset di;
    TimeOffset pd;
    std::string src;
    std::string dest;

    //getting each line of the input
    while (std::getline(is, line)) {
        //handling comments and empty lines
        if (line.empty() or line.substr(0,1) == ";") {
            continue;
        }
        //calling the parse_line function
        ParsedLineData pdata = parse_line(line);

        //handling different cases based on element type
        switch (pdata.element_type) {
            case ElementType::RAMP:
                //converting the parameters to correct types
                //and adding the ramp to the factory
                id = std::stoi(pdata.parameters.at("id"));
                di = std::stoi(pdata.parameters.at("delivery-interval"));
                F.add_ramp(Ramp(id, di));
            case ElementType::WORKER:
                //converting the parameters to correct types
                //and adding the worker to the factory
                id = std::stoi(pdata.parameters.at("id"));
                pd = std::stoi(pdata.parameters.at("processing-time"));
                if (pdata.parameters.at("queue-type")=="FIFO") {
                    F.add_worker(Worker(id, pd, std::make_unique<PackageQueue>(QueueType::Fifo)));
                }
                else if (pdata.parameters.at("queue-type")=="LIFO") {
                    F.add_worker(Worker(id, pd,std::make_unique<PackageQueue>(QueueType::Lifo)));
                }
                //error handling
                else {
                    throw std::invalid_argument("No matching queue type");
                }
            case ElementType::STOREHOUSE:
                //converting the parameters to correct types
                //and adding the storehouse to the factory
                id = std::stoi(pdata.parameters.at("id"));
                F.add_storehouse(Storehouse(id));
            case ElementType::LINK:
                //creation of a connection between two nodes

                //defining helpful variables
                std::map<std::string,ElementType> nodes{
            {"ramp",ElementType::RAMP},
            {"worker",ElementType::WORKER},
            {"storehouse",ElementType::STOREHOUSE}
                };
                src = pdata.parameters.at("src");
                dest = pdata.parameters.at("dest");
                std::istringstream src_stream(src);
                std::istringstream dest_stream(dest);
                std::string src_node;
                std::string dest_node;
                std::string temp;
                ElementID src_id;
                ElementID dest_id;

                //dividing the sources and destinations
                //into their element types and respective ids
                std::getline(src_stream, src_node, '-');
                std::getline(dest_stream, dest_node, '-');
                std::getline(src_stream, temp, ' ');
                src_id = std::stoi(temp);
                std::getline(dest_stream, temp, ' ');
                dest_id = std::stoi(temp);
                ElementType src_type;
                ElementType dest_type;
                try {
                    src_type = nodes.at(src_node);
                    dest_type = nodes.at(dest_node);
                }
                //error handling in case
                //of an incorrect parameter
                catch (const std::out_of_range&) {
                    std::cerr <<"No matching node type";
                }
                //defining behavior based on source type
                switch (src_type) {
                    case ElementType::RAMP:
                        //finding the correct ramp in the Factory object
                        auto R_src = F.find_ramp_by_id(src_id);
                        //defining behavior based on destination type
                        switch (dest_type) {
                            case ElementType::WORKER:
                                //finding the correct worker and assigning it as a ramp's receiver
                                auto W_dest = F.find_worker_by_id(dest_id);
                                R_src->receiver_preferences.add_receiver(&(*W_dest));
                            case ElementType::STOREHOUSE:
                                //finding the correct storehouse and assigning it as a ramp's receiver
                                auto S_dest = F.find_storehouse_by_id(dest_id);
                                R_src->receiver_preferences.add_receiver(&(*S_dest));
                            case default:
                                //error handling
                                throw std::invalid_argument("No matching receiver (RAMP can't be a receiver)");
                        }
                    case ElementType::WORKER:
                        //finding the correct worker in the Factory object
                        auto W_src = F.find_worker_by_id(src_id);
                        //defining behavior based on destination type
                        switch (dest_type) {
                            case ElementType::WORKER:
                                //finding the correct worker and assigning it as a worker's receiver
                                auto W_dest = F.find_worker_by_id(dest_id);
                                W_src->receiver_preferences.add_receiver(&(*W_dest));
                            case ElementType::STOREHOUSE:
                                //finding the correct storehouse and assigning it as a worker's receiver
                                auto S_dest = F.find_storehouse_by_id(dest_id);
                                S_dest->receiver_preferences.add_receiver(&(*S_dest));
                            case default:
                                //error handling
                                throw std::invalid_argument("No matching receiver (RAMP can't be a receiver)");
                        }
                    case default:
                        //error handling
                        throw std::invalid_argument("No matching source (STOREHOUSE can't be a sender)");
                }

            case default:
                //error handling
                //but impossible to get
                //since this error should already be handled
                //in parse_line function
                throw std::invalid_argument("HOW???");

        }
    }
    //returning the complete factory
    return F;
}

//helper functions for save_factory_structure
std::string receiver_save_func(auto &rp) {
    //defining the logic of outputting receiver
    //as to not repeat the code twice
    std::ostringstream ss;
    ss <<"\tReceivers:\n";
    std::map<ReceiverType,std::string> receivers{
        {ReceiverType::WORKER,"worker"},
        {ReceiverType::STOREHOUSE,"storehouse"}
        };
    for (auto it = rp.begin(); it != rp.end(); ++it) {
        ReceiverType r = it->first->get_receiver_type();
        ElementID id = it->first->get_id();
        ss<<"\t\t"<<receivers.at(r)<<" #"<<id<<"\n";
    }
    return ss.str();
}

void ramp_save_func (Ramp &ramp, std::ostream &os) {
    //defining the logic of outputting ramp specs
    //to pass in std::for_each function
    os << "LOADING RAMP #" << ramp.get_id() << "\n";
    os << "\tDelivery interval: " << ramp.get_delivery_interval() << "\n" ;
    os << receiver_save_func(ramp.get_preferences()) << "\n";
}
void worker_save_func (Worker &worker, std::ostream &os) {
    //defining the logic of outputting worker specs
    //to pass in std::for_each function
    os << "WORKER #" << worker.get_id() << "\n";
    os << "\tProcessing time:" << worker.get_processing_duration() << "\n";
    os << "\tQueue type:" << worker.get_queue() << "\n";
    os << receiver_save_func(worker.get_preferences()) << "\n";
}

void storehouse_save_func (Storehouse &storehouse, std::ostream &os) {
    //defining the logic of outputting storehouse specs
    //to pass in std::for_each function
    os << "STOREHOUSE #" << storehouse.get_id() << "\n";
}

void save_factory_structure (Factory &factory, std::ostream &os) {
    /*function that makes a factory structure report
     *takes an address to the Factory object
     *and the std::ostream address (so it's possible to call on both std::cout and std::ofstream)
     *returns nothing
     */
    os<<"\n== LOADING RAMPS ==\n\n";
    std::for_each(factory.ramp_cbegin,factory.ramp_cend,ramp_save_func);
    os<<"\n== WORKERS ==\n\n";
    std::for_each(factory.worker_cbegin,factory.worker_cend,worker_save_func);
    os<<"\n== STOREHOUSES ==\n\n";
    std::for_each(factory.storehouse_cbegin,factory.storehouse_cend,storehouse_save_func);

}

