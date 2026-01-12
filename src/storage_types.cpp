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

Package PackageQueue::pop() {
    Package p;
  if (qt == QueueType::Fifo) {
      p = std::move(packages.front());
      packages.pop_front();
  }
  else if (qt == QueueType::Lifo) {
      p = std::move(packages.back());
      packages.pop_back();
  }
  else {
    throw std::invalid_argument("Invalid QueueType");
  }
    return p;
  }

void PackageQueue::push( Package&& p) {
  packages.push_front(std::move(p));
}