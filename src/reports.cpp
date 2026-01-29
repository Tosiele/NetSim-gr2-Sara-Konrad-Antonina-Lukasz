#include "reports.hpp"
#include "nodes.hpp"
#include <vector>
#include <algorithm>
#include <map>

// HELPER FUNCTIONS

// queue type -> string

std::string queue_to_string(PackageQueueType type) {
    if (type == PackageQueueType::FIFO) return "FIFO";
    if (type == PackageQueueType::LIFO) return "LIFO";
    return "Unknown";
}

// receiver type -> string
std::string receiver_type_to_string(ReceiverType type) {
    if (type == ReceiverType::WORKER) return "worker";
    if (type == ReceiverType::STOREHOUSE) return "storehouse";
    return "unknown";
}

// storehouse before worker
bool receiver_comparator(const IPackageReceiver* a, const IPackageReceiver* b) {
    ReceiverType ta = a->get_receiver_type();
    ReceiverType tb = b->get_receiver_type();

    if (ta != tb) {
        return ta == ReceiverType::STOREHOUSE;
    }
    // for objects of the same type, younger ID is first
    return a->get_id() < b->get_id();
}


// RAPORT GENERATOR

void structure_report(const Factory& f, std::ostream& os) {
//=================================================
    os << "\n== LOADING RAMPS ==\n";

    //collecting all pointers to ramps in a stdvector
    std::vector<const Ramp*> ramps;
    for (auto it = f.ramp_begin(); it != f.ramp_end(); ++it) {
        ramps.push_back(&(*it)); // &(*it) zamienia iterator na zwykły wskaźnik
    }

    // sorting ramps ID-wise
    std::sort(ramps.begin(), ramps.end(), [](const Ramp* a, const Ramp* b) {
        return a->get_id() < b->get_id();
    });

    // printing
    for (const auto* r : ramps) {
        os << "\nLOADING RAMP #" << r->get_id() << "\n";
        os << "  Delivery interval: " << r->get_delivery_interval() << "\n";
        os << "  Receivers:\n";

        // sorting recievers
        auto receivers_map = r->receiver_preferences_.get_preferences();
        std::vector<const IPackageReceiver*> sorted_receivers;
        
        // vetor of keys (pointers to recievers)
        for (const auto& pair : receivers_map) {
            sorted_receivers.push_back(pair.first);
        }

        // using the helper
        std::sort(sorted_receivers.begin(), sorted_receivers.end(), receiver_comparator);

        // printing
        for (const auto* rec : sorted_receivers) {
            os << "    " << receiver_type_to_string(rec->get_receiver_type()) 
               << " #" << rec->get_id() << "\n";
        }
    }
//=================================================
    os << "\n== WORKERS ==\n";

    // pointers
    std::vector<const Worker*> workers;
    for (auto it = f.worker_begin(); it != f.worker_end(); ++it) {
        workers.push_back(&(*it));
    }

    // sort by ID
    std::sort(workers.begin(), workers.end(), [](const Worker* a, const Worker* b) {
        return a->get_id() < b->get_id();
    });

    // print
    for (const auto* w : workers) {
        os << "\nWORKER #" << w->get_id() << "\n";
        os << "  Processing time: " << w->get_processing_duration() << "\n";
        os << "  Queue type: " << queue_to_string(w->get_queue_type()) << "\n";
        os << "  Receivers:\n";

        // sorting receivers
        auto receivers_map = w->receiver_preferences_.get_preferences();
        std::vector<const IPackageReceiver*> sorted_receivers;
        for (const auto& pair : receivers_map) {
            sorted_receivers.push_back(pair.first);
        }
        std::sort(sorted_receivers.begin(), sorted_receivers.end(), receiver_comparator);

        for (const auto* rec : sorted_receivers) {
            os << "    " << receiver_type_to_string(rec->get_receiver_type()) 
               << " #" << rec->get_id() << "\n";
        }
    }

//=================================================
    os << "\n== STOREHOUSES ==\n";

    // pointers
    std::vector<const Storehouse*> storehouses;
    for (auto it = f.storehouse_begin(); it != f.storehouse_end(); ++it) {
        storehouses.push_back(&(*it));
    }

    // ID sort
    std::sort(storehouses.begin(), storehouses.end(), [](const Storehouse* a, const Storehouse* b) {
        return a->get_id() < b->get_id();
    });

    // print (no receivers in storehouse obv)
    for (const auto* s : storehouses) {
        os << "\nSTOREHOUSE #" << s->get_id() << "\n";
    }
}

//=================================================
//===============TURN REPORTS======================
//=================================================

void turn_report(const Factory& f, Time t, std::ostream& os) {
    os << "=== [ Turn: " << t << " ] ===\n";

//=================================================
    os << "\n== WORKERS ==\n";

    std::vector<const Worker*> workers;
    for (auto it = f.worker_begin(); it != f.worker_end(); ++it) {
        workers.push_back(&(*it));
    }
    std::sort(workers.begin(), workers.end(), [](const Worker* a, const Worker* b) {
        return a->get_id() < b->get_id();
    });

    for (const auto* w : workers) {
        os << "\nWORKER #" << w->get_id() << "\n";

        os << "  PBuffer: ";
        auto& p_buffer = w->get_processing_buffer();
        if (p_buffer.has_value()) {
            Time progress = t - w->get_package_processing_start_time() + 1;
            os << "#" << p_buffer->get_id() << " (pt = " << progress << ")";
        } else {
            os << "(empty)";
        }
        os << "\n";

        os << "  Queue: ";
        if (w->begin() == w->end()) {
            os << "(empty)";
        } else {

            bool first = true;
            for (const auto& pkg : *w) { 
                if (!first) os << ", "; 
                os << "#" << pkg.get_id();
                first = false;
            }
        }
        os << "\n";


        os << "  SBuffer: ";
        auto& s_buffer = w->get_sending_buffer();
        if (s_buffer.has_value()) {
            os << "#" << s_buffer->get_id();
        } else {
            os << "(empty)";
        }
        os << "\n";
    }

//=================================================
    os << "\n== STOREHOUSES ==\n";

    std::vector<const Storehouse*> storehouses;
    for (auto it = f.storehouse_begin(); it != f.storehouse_end(); ++it) {
        storehouses.push_back(&(*it));
    }
    std::sort(storehouses.begin(), storehouses.end(), [](const Storehouse* a, const Storehouse* b) {
        return a->get_id() < b->get_id();
    });

    for (const auto* s : storehouses) {
        os << "\nSTOREHOUSE #" << s->get_id() << "\n";
        
        os << "  Stock: ";
        if (s->begin() == s->end()) {
            os << "(empty)";
        } else {
            bool first = true;
            for (const auto& pkg : *s) {
                if (!first) os << ", ";
                os << "#" << pkg.get_id();
                first = false;
            }
        }
        os << "\n";
    }
}