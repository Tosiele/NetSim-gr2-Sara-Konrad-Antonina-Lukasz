#ifndef PACKAGE_HPP
#define PACKAGE_HPP

#include <set>
#include <iostream>
using ElementID = long long;

class Package {
public:
    Package();//base constructor

    Package(ElementID id); // 2. Constructor based on ElementID (z konkretnym ID)

    Package(Package&& other) noexcept; // 3. Constructor based on other Package r-value (przenoszący)

    // --- METODY I OPERATORY ---

    // Package equality operator (czy paczki są takie same?)
    bool operator==(const Package& other) const; // Zadanie: Equality operator

    // Metoda pobierająca ID
    ElementID get_id() const;

    // --- DESTRUKTOR (Zadanie #4) ---
    ~Package();

private:
    ElementID id_; // Tutaj przechowujemy ID konkretnej paczki

    // --- POLA STATYCZNE (Zadanie: assigned_IDs list, freed_IDs list) ---
    // Są statyczne (static), bo lista zajętych ID jest wspólna dla WSZYSTKICH paczek
    static std::set<ElementID> assigned_IDs;
    static std::set<ElementID> freed_IDs;
};

#endif
