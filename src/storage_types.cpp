#include <iostream>
#include "storage_types.hpp"

void return_queue_type(QueueType t) {
    switch (t) {
        case QueueType::Fifo:
            std::cout << "fifo" << std::endl;
        break;
        case QueueType::Lifo:
            std::cout << "lifo" << std::endl;
        break;
        default:
            throw std::invalid_argument("Invalid QueueType");
        break;
    }
}