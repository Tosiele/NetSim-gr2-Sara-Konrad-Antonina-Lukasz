//
// Created by konfe on 12.01.2026.
//

#ifndef NODES_HPP
#define NODES_HPP
#include "storage_types.hpp"
class IPackageReceiver {
  public:
    virtual void receive_package(Package& package) = 0;
    IPackageStockpile::const_iterator begin() {return packages.begin();}
    IPackageStockpile::const_iterator end() {return packages.end();}
    const IPackageStockpile::const_iterator cbegin() const {return packages.cbegin();}
    const IPackageStockpile::const_iterator cend() const {return packages.cend();}
    ElementID get_id() const {return packages.front().id;}
  private:
    std::list<Package> packages;
  };

#endif //NODES_HPP
