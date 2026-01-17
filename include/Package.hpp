#ifndef PACKAGE_HPP
#define PACKAGE_HPP

#include <set>
#include <iostream>
#include "types.hpp"
//using ElementID = long long;

class Package {
public:
    Package() {id_=1;};//base constructor

    Package(ElementID id) {id_=id;}; // 2. Constructor based on ElementID (z konkretnym ID)

    Package(Package&& other) =default;; // 3. Constructor based on other Package r-value (przenoszący)

    // --- METODY I OPERATORY ---

    Package& operator=(Package&&) = default;

    // Package equality operator (czy paczki są takie same?)
    bool operator==(const Package& other) const {return get_id()==other.get_id();}; // Zadanie: Equality operator

    // Metoda pobierająca ID
    ElementID get_id() const {return id_;};

    // --- DESTRUKTOR (Zadanie #4) ---
    ~Package() = default;

private:
    ElementID id_; // Tutaj przechowujemy ID konkretnej paczki

    // --- POLA STATYCZNE (Zadanie: assigned_IDs list, freed_IDs list) ---
    // Są statyczne (static), bo lista zajętych ID jest wspólna dla WSZYSTKICH paczek
    static std::set<ElementID> assigned_IDs;
    static std::set<ElementID> freed_IDs;
};

#endif