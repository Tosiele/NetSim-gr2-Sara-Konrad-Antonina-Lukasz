#include "../include/factory.hpp"
#include "nodes.hpp"
#include "types.hpp"

ParsedLineData parse_line(const std::string &line) {
    ParsedLineData parsed_line;
    std::vector<std::string> tokens;
    std::string token;

    std::istringstream token_stream(line);
    char delimiter = ' ';

    std::getline(token_stream, token, delimiter);
    std::map<std::string,ElementType> tags {
        {"LOADING_RAMP",ElementType::RAMP},
        {"WORKER",ElementType::WORKER},
        {"STOREHOUSE",ElementType::STOREHOUSE},
        {"LINK",ElementType::LINK}
    };

    if (tags.find(token) != tags.end()) {
        parsed_line.element_type = tags.at(token);
    }
    else {
        throw std::invalid_argument("No matching tag");
    }

    std::string key;
    std::string value;
    while (std::getline(token_stream, token, delimiter)) {
        std::istringstream keyval_stream(token);
        char delim = '=';
        std::getline(keyval_stream, key, delim);
        std::getline(keyval_stream, value, delimiter);
        parsed_line.parameters.emplace(key, value);
    }

    return parsed_line;
}

Factory load_factory_structure(std::istream &is) {
    Factory F{};
    std::string line;

    ElementID id;
    TimeOffset di;
    TimeOffset pd;
    std::string src;
    std::string dest;

    while (std::getline(is, line)) {
        if (line.empty() or line.substr(0,1) == ";") {
            continue;
        }
        ParsedLineData pdata = parse_line(line);
        switch (pdata.element_type) {
            case ElementType::RAMP:
                id = std::stoi(pdata.parameters.at("id"));
                di = std::stoi(pdata.parameters.at("delivery-interval"));
                F.add_ramp(Ramp(id, di));
            case ElementType::WORKER:
                id = std::stoi(pdata.parameters.at("id"));
                pd = std::stoi(pdata.parameters.at("processing-time"));
                if (pdata.parameters.at("queue-type")=="FIFO") {
                    F.add_worker(Worker(id, pd, std::make_unique<PackageQueue>(QueueType::Fifo)));
                }
                else if (pdata.parameters.at("queue-type")=="LIFO") {
                    F.add_worker(Worker(id, pd,std::make_unique<PackageQueue>(QueueType::Lifo)));
                }
            case ElementType::STOREHOUSE:
                id = std::stoi(pdata.parameters.at("id"));
                F.add_storehouse(Storehouse(id));
            case ElementType::LINK:
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
                catch (const std::out_of_range&) {
                    std::cerr <<"No matching node type";
                }
                //probably should add try/catch
                //when the id is not correct
                //but dont know what error type is it
                switch (src_type) {
                    case ElementType::RAMP:
                        auto R_src = F.find_ramp_by_id(src_id);
                        switch (dest_type) {
                            case ElementType::WORKER:
                                auto W_dest = F.find_worker_by_id(dest_id);
                                R_src->receiver_preferences.add_receiver(&(*W_dest));
                            case ElementType::STOREHOUSE:
                                auto S_dest = F.find_storehouse_by_id(dest_id);
                                R_src->receiver_preferences.add_receiver(&(*S_dest));
                            case default:
                                throw std::invalid_argument("No matching receiver (RAMP can't be a receiver)");
                        }
                    case ElementType::WORKER:
                        auto W_src = F.find_worker_by_id(src_id);
                        switch (dest_type) {
                            case ElementType::WORKER:
                                auto W_dest = F.find_worker_by_id(dest_id);
                                W_src->receiver_preferences.add_receiver(&(*W_dest));
                            case ElementType::STOREHOUSE:
                                auto S_dest = F.find_storehouse_by_id(dest_id);
                                S_dest->receiver_preferences.add_receiver(&(*S_dest));
                            case default:
                                throw std::invalid_argument("No matching receiver (RAMP can't be a receiver)");
                        }
                    case default:
                        throw std::invalid_argument("No matching source (STOREHOUSE can't be a sender)");
                }

            case default:
                throw std::invalid_argument("HOW???");

        }
    }
    return F;
}

void ramp_save_func (Ramp &ramp, std::ostream &os) {
    os << "LOADING RAMP #" << ramp.get_id() << "\n";
    os << "\tDelivery interval: " << ramp.get_delivery_interval() << "\n" ;
    os << receiver_save_func(ramp.get_receiver_preferences()) << "\n";
}

void save_factory_structure (Factory &factory, std::ostream &os) {
    os<<"\n== LOADING RAMPS ==\n\n";
    std::for_each(factory.ramp_cbegin,factory.ramp_cend,ramp_save_func);
    os<<"\n== WORKERS ==\n\n";
    std::for_each(factory.worker_cbegin,factory.worker_cend,worker_save_func);
    os<<"\n== STOREHOUSES ==\n\n";
    std::for_each(factory.storehouse_cbegin,factory.storehouse_cend,storehouse_save_func);

}
