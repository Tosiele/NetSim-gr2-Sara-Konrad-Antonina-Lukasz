//
// Created by konfe on 07.01.2026.
//

#ifndef STORAGE_TYPES_HPP
#define STORAGE_TYPES_HPP
#include <iostream>
#include "types.hpp"
enum class QueueType {
    Fifo, Lifo

};
void return_queue_type(QueueType t) {}

class IPackageStockpile {
  public:
    using const_iterator = std::list::const_iterator

    virtual void push(const Package&& p) = 0;
    virtual ~IPackageStockpile() {}

    const_iterator begin() { return std::list::begin(packages); }
    const_iterator end() { return std::list::end(packages); }
    const const_iterator cbegin() const { return std::list::cbegin(packages); }
    const const_iterator cend() const { return std::list::cend(packages); }

    int size() const { return std::list::size(packages); }
    int empty() const { return std::list::empty(packages); }
  };
  private:
    std::list<IPackageStockpile*> packages;


#endif //STORAGE_TYPES_HPP
