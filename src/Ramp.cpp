#include "Ramp.hpp"
#include "Package.hpp"
#include <utility>

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
