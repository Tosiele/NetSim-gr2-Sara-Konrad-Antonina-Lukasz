#ifndef RAMP_HPP
#define RAMP_HPP

#include "nodes.hpp"
#include "types.hpp"

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
#endif // RAMP_HPP