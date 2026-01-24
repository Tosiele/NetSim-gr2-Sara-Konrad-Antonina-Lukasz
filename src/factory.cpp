//
// Created by konfe on 24.01.2026.
//

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


    if (token == "LOADING_RAMP") {
        parsed_line.element_type = ElementType::RAMP;
    }
    else if (token == "WORKER") {
        parsed_line.element_type = ElementType::WORKER;
    }
    else if (token == "STOREHOUSE") {
        parsed_line.element_type = ElementType::STOREHOUSE;
    }
    else if (token == "LINK") {
        parsed_line.element_type = ElementType::LINK;
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
                //TODO
            case default:
                throw std::invalid_argument("HOW???");

        }
    }
    return F;
}
