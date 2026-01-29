//
// Created by konfe on 12.01.2026.
//

#ifndef NODES_HPP
#define NODES_HPP
#include "storage_types.hpp"
#include <map>
#include <functional>
#include <cmath>
#include <memory>
#include <optional>
#include "helpers.hpp"
/*
 *Includes all classes that represent different nodes in the factory simulation
 *along with some base classes that help in their implementation:
 *enum class ReceiverType
 *interface IPackageReceiver
 *class ReceiverPreferences
 *class PackageSender
 *class Storehouse
 *class Ramp
 *class Worker
 */



enum class ReceiverType {
  //enumeration class that includes types of receivers
    WORKER, STOREHOUSE
};


template<class Node>
class NodeCollection {
  public:
      using container_t = typename std::list<Node>;
      using iterator = typename container_t::iterator;
      using const_iterator = typename container_t::const_iterator;
      // methods to add and remove nodes
      void add(const Node&& node) { container.push_back(std::move(node)); }
      void remove_by_id(ElementID id) {
          container.remove_if([id](const Node& node){ return node.get_id() == id; });
      }
      // methods to find nodes by id
      iterator find_by_id(ElementID id) {
          return std::find_if(container.begin(), container.end(),
                              [id](const Node& node){ return node.get_id() == id; });
      }
      const_iterator find_by_id(ElementID id) const {
          return std::find_if(container.cbegin(), container.cend(),
                              [id](const Node& node){ return node.get_id() == id; });
      }
      // iterator methods
      iterator begin() { return container.begin(); }
      iterator end() { return container.end(); }
      const_iterator cbegin() const { return container.cbegin(); }
      const_iterator cend() const { return container.cend(); }
  private:
      container_t container;
};


class IPackageReceiver {
  //interface that defines basic receiving methods for child classes
  public:
  //virtual method receive_package to receive the given package
  virtual void receive_package(Package&& package) = 0;
  //virtual methods [c]begin and [c]end
  virtual IPackageStockpile::const_iterator begin() =0;
  virtual IPackageStockpile::const_iterator end() =0;
  virtual const IPackageStockpile::const_iterator cbegin() const =0;
  virtual const IPackageStockpile::const_iterator cend() const =0;
  //virtual method get_id to return ElementID
  virtual ElementID get_id() const =0;
  //virtual method get_receiver_type to return type of receiver
  virtual ReceiverType get_receiver_type() const = 0;
  //default virtual destructor
  virtual ~IPackageReceiver() = default;

  };

class ReceiverPreferences {
  //class that defines how a sending node chooses a receiving node
  public:
  //aliases for std::map and its const iterator
    using preferences_t = std::map<IPackageReceiver*, double>;
    using const_iterator = preferences_t::const_iterator;
  //basic constructor that takes the default probability_generator as the probability generator
    ReceiverPreferences() {pg = probability_generator;};
  //constructor that takes given probability generator
    ReceiverPreferences(ProbabilityGenerator prob_gen) {pg=prob_gen;};

  //method add_receiver to add a receiver to the receiver pool
    void add_receiver(IPackageReceiver* receiver);
  //method remove_receiver to remove a receiver from the receiver pool
    void remove_receiver(IPackageReceiver* receiver);
  //method choose_receiver to choose a receiver from the receiver pool
    IPackageReceiver* choose_receiver();
  //method get_preferences that returns the receiver pool
    preferences_t& get_preferences() {return prefs;};

  //methods [c]begin and [c]end that allow read-only access
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
     void receive_package(Package&& package) override {;};
    IPackageStockpile::const_iterator begin() override {return packages.begin();}
    IPackageStockpile::const_iterator end() override {return packages.end();}
    const IPackageStockpile::const_iterator cbegin() const override {return packages.cbegin();}
    const IPackageStockpile::const_iterator cend() const override {return packages.cend();}
    ElementID get_id() const override {return 1;}
    ReceiverType get_receiver_type() const override {return ReceiverType::WORKER;};
  private:
  std::list<Package> packages;
};


class PackageSender {
  //class that defines basic sending methods for factory nodes
  public:
  //basic constructor
  PackageSender() = default;
  //moving constructor
  PackageSender(PackageSender&&) = default;
  //method send_package that sends the package to a receiver
  void send_package();
  //method get_sending_buffer that returns the current state of the std::optional buffer
  std::optional<Package>& get_sending_buffer(){return buffer;}
  ReceiverPreferences receiver_preferences;
  protected:
  //method push_package that inputs the package into the buffer
  void push_package(Package&& package);
  private:
  std::optional<Package> buffer;
};

class Storehouse:public IPackageReceiver {
  //class that defines tha behavior of a storehouse: receiving and storing items and their read-only access
  public:
  //constructor that takes an ElementID and a unique pointer of type IPackageStockpile(with default argument as Fifo queue)
  Storehouse(ElementID i, std::unique_ptr<IPackageStockpile> s = std::make_unique<PackageQueue>(QueueType::Fifo)) {id=i;packages=std::move(s);};
  //method get_receiver_type that returns the Storehouse ReceiverType
  ReceiverType get_receiver_type() const override {return ReceiverType::STOREHOUSE;};
  //method receive_package that inputs the package into the storehouse queue
  void receive_package(Package&& package) override;
  //methods [c]begin and [c]end that allow read-only access to the queue
  IPackageStockpile::const_iterator begin() override {return packages->begin();}
  IPackageStockpile::const_iterator end() override {return packages->end();}
  const IPackageStockpile::const_iterator cbegin() const override {return packages->cbegin();}
  const IPackageStockpile::const_iterator cend() const override {return packages->cend();}
  //method get_id that returns ElementID of a Storehouse
  ElementID get_id() const override {return id;}
  private:
  ElementID id;
  std::unique_ptr<IPackageStockpile> packages;
};

//ONLY FOR TESTS
class ExampleSender:public PackageSender {
  public:
  ExampleSender() = default;
  void pusher() {push_package(Package(7));}
};

/*
CLASS WORKER
A constructor that takes a variable pd of type TimeOffset
 (that says how long is his processing time) and a pointer to
 IPackageQueue using smart pointers (std::unique_ptr),
 because we don't know which queue type he will get

method do_works that takes current simulation time and saves it
 to know when his processing will be done using pd

method get_processing_duration that returns pd

method get_package_processing_start_time that returns when the package processing started

*/

class Worker: public IPackageReceiver, public PackageSender{
    public:
    Worker(ElementID id, TimeOffset pd, std::unique_ptr<IPackageQueue> queue);

    void do_work(Time t);

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
    QueueType get_queue_type() const;

    private:
    ElementID id_;
    TimeOffset pd_;

    std::unique_ptr<IPackageQueue> queue_;
    std::optional<Package> currently_processed_package_; // optional so it can be also empty

    Time t_processing_start_;


};


/*
CLASS RAMP
Inherits from PackageSender
Constructor takes id of type ElementID and di of type TimeOffset

method deliver_goods called in simulation that takes current simulation time and determines if the new package should appear. If the package appears, it should be immediately put in the buffer of PackageSender
method get_delivery_interval that returns di
method get_id that returns current ElementID
*/

class Ramp : public PackageSender{
public:
  // constructor
  Ramp(ElementID id, TimeOffset di);

  // methods
  void deliver_goods(Time t);

  TimeOffset get_delivery_interval() const;

  ElementID get_id() const;
  // destructor
  ~Ramp() = default;

private:
  ElementID id_;
  TimeOffset di_;
  Time t_last_delivery_;
};

#endif //NODES_HPP
