//
// Created by konfe on 07.01.2026.
//

#ifndef STORAGE_TYPES_HPP
#define STORAGE_TYPES_HPP
#include <iostream>
#include "types.hpp"
#include <list>
#include "Package.hpp"
  enum class QueueType {
    Fifo, Lifo
};

void return_queue_type(QueueType t);

class IPackageStockpile {
  public:
    using const_iterator = std::list<IPackageStockpile*>::const_iterator;
    virtual void push(Package&& p) = 0;
    virtual ~IPackageStockpile() = default;

    const_iterator begin() { return packages.begin(); }
    const_iterator end() { return packages.end(); }
    const const_iterator cbegin() const { return packages.cbegin(); }
    const const_iterator cend() const { return packages.cend(); }

    const unsigned long long size() const { return packages.size(); }
    const bool empty() const { return packages.empty(); }
  private:
  std::list<IPackageStockpile*> packages;
};

class IPackageQueue : public IPackageStockpile {
  public:
    virtual Package pop() = 0;
    const auto get_queue_type() { return return_queue_type(qt); }
    virtual ~IPackageQueue() {}
  private:
    QueueType qt;
};

class PackageQueue : public IPackageQueue {
  public:
    explicit PackageQueue(const QueueType t) : qt(t) { qt = t; }
    Package pop() override;
    void push(Package&& p) override;
  private:
    QueueType qt;
    std::list<Package> packages;
};


#endif //STORAGE_TYPES_HPP
