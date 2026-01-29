#ifndef WORKER_hpp
#define WORKER_hpp

#include <iostream>
#include <nodes.hpp>
#include <types.hpp>
#include "Package.hpp" 
#include <memory>   // For std::unique_ptr
#include <optional> // For std::optional

/*

A constructor that takes a variable pd of type TimeOffset
 (that says how long is his processing time) and a pointer to 
 IPackageQueue using smart pointers (std::unique_ptr), 
 because we don't know which queue type he will get

method do_works that takes current simulation time and saves it
 to know when his processing will be done using pd

method get_processing_duration that returns pd

method get_package_processing_start_time that returns when the package processing started



An interface that takes in Packages and allows their identification. Add:

-recieve_package method that recieves rvalue of an object Package
-begin and end read_only methods (also const versions)
-get_id method that returns ElementID
-get_receiver_type() returns proper etiquette from enum 
class ReceiverType (more in project Fabryka in Badanie spójności sieci)
-get_type method (maybe?)
Use type IPackageStockpile::const_iterator
*/

class Worker: public IPackageReceiver, public PackageSender{
    public:
    Worker(ElementID id, TimeOffset pd, std::unique_ptr<IPackageQueue> queue);

    void do_works(Time t);

    TimeOffset get_processing_duration() const;

    Time get_package_processing_start_time() const;

    //abstract methods from IPackageReciever
    void receive_package(Package&& p) override;//fixed a typo reCEive

    ElementID get_id() const override;

    ReceiverType get_receiver_type() const override;

    IPackageStockpile::const_iterator begin() override;
    IPackageStockpile::const_iterator end() override;
    const IPackageStockpile::const_iterator cbegin() const override;
    const IPackageStockpile::const_iterator cend() const override;

    // MODIFICATION TO SHARE DATA
    std::optional<Package> const& get_processing_buffer() const;
    
    private:
    ElementID id_;
    TimeOffset pd_;

    std::unique_ptr<IPackageQueue> queue_;
    std::optional<Package> currently_processed_package_; // optional so it can be also empty

    Time t_processing_start_;

    
};

#endif