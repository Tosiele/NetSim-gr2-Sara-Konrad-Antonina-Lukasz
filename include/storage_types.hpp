//
// Created by konfe on 07.01.2026.
//

#ifndef STORAGE_TYPES_HPP
#define STORAGE_TYPES_HPP
#include <iostream>
#include <list>
#include "Package.hpp"
/*
 *Includes all classes needed in initializing queues of packages:
 * enum class QueueType (also return_queue_type method)
 * interface IPackageStockpile
 * interface IPackageQueue
 * class PackageQueue
 */
  enum class QueueType {
    //enumeration class QueueType
    //includes only Fifo and Lifo queues, but can be modified if needed
    Fifo, Lifo
};

void return_queue_type(QueueType t); //function that returns queue type of object

class IPackageStockpile {
  //interface that initializes basic item storing methods
  public:
  //alias for a const iterator, useful in defining begin and end methods
    using const_iterator = std::list<Package>::const_iterator;
  //virtual method push for adding a Package into the storage
    virtual void push(Package&& p) = 0;
  //default virtual destructor
    virtual ~IPackageStockpile() = default;

  //virtual [c]begin and [c]end methods
    virtual const_iterator begin() =0;
    virtual const_iterator end() =0;
    virtual const const_iterator cbegin() const =0;
    virtual const const_iterator cend() const =0;

  //virtual method size for accessing the number of stored values
    virtual const unsigned long long size() const =0;
  //virtual method empty for checking if the storage is empty
    virtual const bool empty() const =0;
};

class IPackageQueue : public IPackageStockpile {
  //interface that inherits from IPackageStockpile and adds method that expand the item storage into a queue
  public:
  //virtual method pop for accessing and removing an element from the queue
    virtual Package pop() = 0;
  //virtual method get_queue_type for accessing the queue type
    virtual QueueType get_queue_type() const =0;
  //default virtual destructor
    virtual ~IPackageQueue() {}
};

class PackageQueue : public IPackageQueue {
//class that inherits from IPackageQueue and implements all the methods as non-virtual
  public:
  //explicit constructor that takes QueueType and assigns it to a private field
    explicit PackageQueue(const QueueType t) : qt(t) { qt = t; }
  //implementation of virtual method pop
    Package pop() override;
  //implementation of virtual method push
    void push(Package&& p) override;

  //implementation of virtual method get_queue_type, calls global return_queue_type function
  QueueType get_queue_type() const override { return qt; }

  //implementation of virtual methods [c]begin and [c]end, allow for read-only access of the queue
  const_iterator begin()  override { return packages.begin(); }
  const_iterator end()  override { return packages.end(); }
  const const_iterator cbegin() const override { return packages.cbegin(); }
  const const_iterator cend() const override { return packages.cend(); }

  //implementation of virtual method size that returns the size of the package list
  const unsigned long long size() const override { return packages.size(); }
  //implementation of virtual method empty that returns true if the list has no elements, otherwise false
  const bool empty() const override { return packages.empty(); }
  private:
    QueueType qt;
    std::list<Package> packages;
};


#endif //STORAGE_TYPES_HPP
