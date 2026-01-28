#include "Worker.hpp"
#include "Package.hpp"
#include "storage_types.hpp"
#include <iostream>

#constructor
Worker::Worker(ElementID id, TimeOffset pd, std::unique_ptr<IPackageQueue> queue):
    // using initializing list for better efficiency
    id_(id),
    pd_(pd),
    queue_(std::move(queue)), // moving, because smart pointer cant be copied
    t_processing_start_(0) // starting time should be 0
    {       
    }

void Worker::do_works(Time t) { //method do_works that takes current simulation time and saves it 
    // to know when his processing will be done using pd
    if (currently_processed_package_) {
    if (t-t_processing_start_+1 >= pd_){ // current round - round when process was started 
    // must be increased by one, as we count every begun round seperate. (5-2 means we started at no.2 so process lasts 2, 3, 4, 5 rounds now)

    push_package(std::move(*currently_processed_package_)); //package goes further
    currently_processed_package_.reset();
}
    }

if (!currently_processed_package_ && !queue_->empty()) { // empty-handed worker and queue not empty
    currently_processed_package_ = queue_->pop(); // start a package, save the time
    t_processing_start_ = t;
}

TimeOffset Worker::get_processing_duration()const{
    return pd_;
}

Time Worker::get_package_processing_start_time()const{
    return t_processing_start_;
}

    //abstract methods from IPackageReciever
void Worker::receive_package(Package&& p){
    queue_->push(std::move(p));
}

ElementID Worker::get_id() const{
    return id_;
}
ReceiverType Worker::get_receiver_type()const{
    return RecieverType::WORKER;
}

    // iterators
IPackageStockpile::const_iterator Worker::begin() const {
    return queue_->begin();
}
IPackageStockpile::const_iterator Worker::end() const {
    return queue_->end();
}
IPackageStockpile::const_iterator Worker::cbegin() const {
    return queue_->cbegin();
}
IPackageStockpile::const_iterator Worker::cend() const {
    return queue_->cend();
}

std::optional<Package> const& Worker::get_processing_buffer() const {
    return currently_processed_package_;
}
