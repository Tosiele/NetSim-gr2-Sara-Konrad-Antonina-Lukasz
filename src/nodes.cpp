//
// Created by konfe on 12.01.2026.
//
#include <iostream>
#include <map>
#include "nodes.hpp"




void ReceiverPreferences::add_receiver(IPackageReceiver* receiver) {
  /*method that adds the receiver to the receiver pool
   and updates the map values accordingly
   takes a pointer to IPackageReceiver
   returns nothing
   */
  prefs.insert(std::pair{receiver,1});
  if (prefs.size()>1) {
    double val = 1.0/static_cast<double>(prefs.size());
    for (auto it = prefs.begin(); it != prefs.end(); ++it) {
      it->second=val;
    }
  }
}
void ReceiverPreferences::remove_receiver(IPackageReceiver* receiver) {
  /*method that removes the receiver from the receiver pool
 and updates the map values accordingly
 takes a pointer to IPackageReceiver
 returns nothing
 */
  prefs.erase(receiver);
  if (not prefs.empty()) {
    double val = 1.0/static_cast<double>(prefs.size());
    for (auto it = prefs.begin(); it != prefs.end(); ++it) {
      it->second=val;
    }
  }
}
IPackageReceiver* ReceiverPreferences::choose_receiver() {
  /*method that chooses the receiver from the receiver pool
   based on the receiver pool and probability_generator
   takes nothing
   returns a pointer to IPackageReceiver
 */
  double rand = pg();
  double threshold = prefs.begin()->second;
  int index = floor(rand/threshold);
  return std::next(prefs.begin(), index)->first;
}

void PackageSender::push_package(Package&& package) {
  /*
   protected method that adds a package into a buffer
   takes Package r-value
   returns nothing
   */
  buffer = std::move(package);
}

void PackageSender::send_package() {
  /*
   method that sends the package to a receiver
   calls choose_receiver method from ReceiverPreferences
   then calls a receive_package method on a chosen receiver
   empties the buffer
   takes nothing
   returns nothing
   */
  IPackageReceiver* R = receiver_preferences.choose_receiver();
  R->receive_package(std::move(buffer.value()));
  buffer.reset();
}

void Storehouse::receive_package(Package&& package) {
  /*
   method that puts a given package into the storehouse queue
   */
  packages->push(std::move(package));
}

Worker::Worker(ElementID id, TimeOffset pd, std::unique_ptr<IPackageQueue> queue):
    // using initializing list for better efficiency
    id_(id),
    pd_(pd),
    queue_(std::move(queue)), // moving, because smart pointer cant be copied
    t_processing_start_(0) // starting time should be 0
    {
    }

void Worker::do_work(Time t) {
    //method do_works that takes current simulation time and saves it
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
    return ReceiverType::WORKER;
}

    // iterators
IPackageStockpile::const_iterator Worker::begin() {
    return queue_->begin();
}
IPackageStockpile::const_iterator Worker::end() {
    return queue_->end();
}
const IPackageStockpile::const_iterator Worker::cbegin() const {
    return queue_->cbegin();
}
const IPackageStockpile::const_iterator Worker::cend() const {
    return queue_->cend();
}

// add get_processing_buffer() module that returns std::optional&
// - the product that's being currently processed
// id, processing pace, list of receivers,
// and queue preference must be available to read
std::optional<Package> const& Worker::get_processing_buffer() const {
    return currently_processed_package_;
}

QueueType Worker::get_queue_type() const {
    return queue_->get_queue_type();
}

Ramp::Ramp(ElementID id, TimeOffset di){
    id_ = id;
    di_ = di;
    t_last_delivery_ = 0;
};


TimeOffset Ramp::get_delivery_interval() const{
    return di_;
};

ElementID Ramp::get_id() const {
    return id_;
}

void Ramp::deliver_goods(Time t) {
    if (t - t_last_delivery_ == di_) {
        Package p;
        push_package(std::move(p));
        t_last_delivery_ = t;
    }
}