//
// Created by konfe on 12.01.2026.
//

#ifndef NODES_HPP
#define NODES_HPP
#include "storage_types.hpp"
#include <map>
#include <functional>
#include <cmath>
#include <optional>

extern std::random_device rd;
extern std::mt19937 rng;
double probability_generator();

enum class ReceiverType {
    WORKER, STOREHOUSE
};


class IPackageReceiver {
  public:
    virtual void receive_package(Package& package) = 0;
    IPackageStockpile::const_iterator begin() {return packages.begin();}
    IPackageStockpile::const_iterator end() {return packages.end();}
    const IPackageStockpile::const_iterator cbegin() const {return packages.cbegin();}
    const IPackageStockpile::const_iterator cend() const {return packages.cend();}
    ElementID get_id() const {return packages.front().get_id();}
    virtual ReceiverType get_receiver_type() const = 0;

  private:
    std::list<Package> packages;
  };

class ReceiverPreferences {
  public:
    using preferences_t = std::map<IPackageReceiver*, double>;
    using const_iterator = preferences_t::const_iterator;

    ReceiverPreferences(ProbabilityGenerator prob_gen) {pg=prob_gen;};

    void add_receiver(IPackageReceiver* receiver);
    void remove_receiver(IPackageReceiver* receiver);
    IPackageReceiver* choose_receiver();
    preferences_t& get_preferences() {return prefs;};

    const_iterator begin() const {return prefs.begin();}
    const_iterator end() const {return prefs.end();}
    const const_iterator cbegin() const {return prefs.cbegin();}
    const const_iterator cend() const {return prefs.cend();}

  private:
    preferences_t prefs;
    ProbabilityGenerator pg;
  };

///ONLY FOR TESTS
class Receiver:public IPackageReceiver {
   public:
     Receiver() = default;
     Receiver& operator=(Receiver& other) = default;
     void receive_package(Package& package) override {;};
     ReceiverType get_receiver_type() const override {return ReceiverType::WORKER;};
};


class PackageSender {
  public:
  PackageSender(PackageSender&&) = default;
  void send_package();
  std::optional<Package>& get_sending_buffer(){return buffer;}
  protected:
  void push_package(Package&& package);
  private:
  ReceiverPreferences receiver_preferences;
  std::optional<Package> buffer;
};

#endif //NODES_HPP
