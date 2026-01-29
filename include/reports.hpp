#ifndef REPORTS_HPP
#define REPORTS_HPP

#include "Factory.hpp"
#include <iostream>

/** Ramps, Workers, Storehouses
 * Sorted by ID.
 * @param f - incpected factory
 * @param os - output stream (either the screen or a file) */

void structure_report(const Factory& f, std::ostream& os);

void turn_report(const Factory& f, Time t, std::ostream& os);

#endif