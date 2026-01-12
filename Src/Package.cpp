#include "Package.hpp"

std::set<ElementID> Package::assigned_IDs;
std::set<ElementID> Package::freed_IDs;

Package::Package(){
     if (!freed_IDs.empty()) {
        id_ = *freed_IDs.begin();
        freed_IDs.erase(id_);
    } else {
        // Ścieżka B: Tworzymy nowe ID
        if (assigned_IDs.empty()) {
            id_ = 1; // Pierwsza paczka w historii
        } else {
            id_ = *assigned_IDs.rbegin() + 1; // Największe ID + 1
        }
    }
    // Zawsze: Zapisujemy, że ID jest zajęte
    assigned_IDs.insert(id_);



}