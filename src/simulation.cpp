//
// Created by konfe on 12.02.2026.
//
#include "../include/simulation.hpp"

void simulate(Factory &f, TimeOffset d, std::function<void(Factory &, Time)> rf) {
    Time t = 1;
    while (t<=d) {
        if (f.is_consistent()==false) {
            throw(std::logic_error("Factory structure is not consistent"));
        }
        f.do_deliveries(t);
        f.do_package_passing();
        f.do_work(t);
        rf(f,t);
        t++;
        }
    }

