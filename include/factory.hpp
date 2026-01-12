#ifndef FACTORY_HPP
#define FACTORY_HPP

#include "storage_types.hpp"
#include "types.hpp"

template<class Node>
class NodeCollection {

    public:
        using container_t = typename std::list<Node>;
        using iterator = typename container_t::iterator;
        using const_iterator = typename container_t::const_iterator;

        iterator begin() { return container.begin(); }
        iterator end() { return container.end(); }
        
        const_iterator begin() const { return container.cbegin(); }
        const_iterator end() const { return container.cend(); }
        
        const_iterator cbegin() const { return container.cbegin(); }
        const_iterator cend() const { return container.cend(); }
    private:

};

class Factory{
    private:
        template<class Node>
        void remove_receiver(NodeCollection<Node>& collection, element_ID id);

    
    public:

    void do_deliveries(Time);
    void do_package_passing();
    void do_work(Time);

    

};

#endif //FACTORY_HPP