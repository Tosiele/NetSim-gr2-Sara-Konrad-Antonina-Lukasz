#include "Package.hpp"
#include <stdexcept>

std::set<ElementID> Package::assigned_IDs;
std::set<ElementID> Package::freed_IDs;

//CONSTRUCTORS
Package::Package(){
     if (!freed_IDs.empty()) {//if theres any free ids, use them

        id_ = *freed_IDs.begin(); //* because its a set and it returns a pointer
        freed_IDs.erase(id_);

    } else {

        if (assigned_IDs.empty()) {
            id_ = 1; //First package ever
        } else {
            id_ = *assigned_IDs.rbegin() + 1; // applying next in order id
        }
    }
    assigned_IDs.insert(id_);
}


Package::Package(ElementID id) {
    // (find(id) goes through all elements and if none of them match id, it returns {the end})

    if (assigned_IDs.find(id) != assigned_IDs.end()){ //if given id is taken already, yell at user
        throw std:: invalid_argument("this ID is already assigned");
    }

    id_ = id;
    assigned_IDs.insert(id_);
    
    if (freed_IDs.find(id) != freed_IDs.end()) {//if given id is in freed - delete it. 
        freed_IDs.erase(id);
    }
}


Package::Package(Package&& other) noexcept {
    id_ = other.id_;
    other.id_ = -1; // destructor will just terminate it
}

// DESTRUCTOR
Package::~Package(){
    if (id_ != 1){
        assigned_IDs.erase(id_);
        freed_IDs.insert(id_);
    }
    // if id is -1, the element is killed anyway, but it wont mess up the sets
}


// METHODS
ElementID Package::get_id() const {
    return id_;
}

bool Package::operator==(const Package& other) const{
    return id_ == other.id_;
}