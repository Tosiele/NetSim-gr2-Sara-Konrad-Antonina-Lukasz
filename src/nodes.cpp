//
// Created by konfe on 12.01.2026.
//
#include <iostream>
#include <map>
#include "nodes.hpp"


//implementation of probability generator function
//Needs to be moved to other file!!!
std::random_device rd;
std::mt19937 rng(rd());
double probability_generator() {
  return std::generate_canonical<double, 10>(rng);
}

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

template<class Node>
class NodeCollection {

    public:
        using container_t = typename std::list<Node>;
        using iterator = typename container_t::iterator;
        using const_iterator = typename container_t::const_iterator;

        iterator begin() { return container.begin(); }
        iterator end() { return container.end(); }
        
        const_iterator begin() const { return container.cbegin(); }
        const_iterator end() const { return container.cend(); }
        
        const_iterator cbegin() const { return container.cbegin(); }
        const_iterator cend() const { return container.cend(); }
    

        NodeCollection<Node>::iterator find_by_id(element_ID id) {

            return std::find_if(container.begin(), container.end(),
                                [id](const Node& ele) {return ele.get_id() == id;});

        }

        NodeCollection<Node>::const_iterator find_by_id(element_ID id) const {

            return std::find_if(container.begin(), container.end(),
                                [id](const Node& ele) {return ele.get_id() == id;});

        }

        void remove_by_id(element_ID id) {
            auto it = find_by_id(id);
            
            if (it != container.end()) {
                container.erase(it);
            }

        }
        
        void add(Node&& node){
            container.push_back(std::move(node));
        }

    private:
        container_t container;
};
