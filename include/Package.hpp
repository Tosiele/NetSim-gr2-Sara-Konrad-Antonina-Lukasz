#ifndef PACKAGE_HPP
#define PACKAGE_HPP

#include <set>
#include <iostream>
using ElementID = long long;

class Package {
public:

//three constructors: base, ID based, r-value based (moving)
    Package();

    Package(ElementID id);

    Package(Package&& other) noexcept;

//methods
    bool operator==(const Package& other) const; //Equality operator

    ElementID get_id() const;// id getter

//destructor
    ~Package();

private:
    ElementID id_;

    // static lists of IDs that are currently in usage or freed
    static std::set<ElementID> assigned_IDs;
    static std::set<ElementID> freed_IDs;
};

#endif
