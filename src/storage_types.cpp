#include <iostream>
#include "storage_types.hpp"

std::string return_queue_type(const QueueType t) {
    /*function that returns queue type name based on QueueType
    takes const QueueType
    returns std::string
    */
    switch (t) {
        case QueueType::Fifo:
            return "FIFO";
        break;
        case QueueType::Lifo:
            return "LIFO";
        break;
        default:
            throw std::invalid_argument("Invalid QueueType");
        break;
    }
}

Package PackageQueue::pop() {
    /*
     Method that outputs a ceirtan object from the queue based on the queue type, and removes it from the queue
     takes nothing
     returns object of type Package
     */
    Package p;
    //implementation for QueueType Fifo
  if (qt == QueueType::Fifo) {
      p = std::move(packages.back());
      packages.pop_back();
  }
    //implementation for QueueType Lifo
  else if (qt == QueueType::Lifo) {
      p = std::move(packages.front());
      packages.pop_front();
  }
    //error if invalid QueueType
  else {
    throw std::invalid_argument("Invalid QueueType");
  }
    return p;
  }

void PackageQueue::push( Package&& p) {
    /*
     Method that places an object into the queue
     takes Package r-value
     returns nothing
     */
  packages.push_front(std::move(p));
}