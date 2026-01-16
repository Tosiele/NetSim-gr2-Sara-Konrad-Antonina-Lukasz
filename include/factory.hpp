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
    

        NodeCollection<Node>::iterator find_by_id(element_ID id) {

            return std::find_if(container.begin(), container.end(),
                                [id](const Node& ele) {return ele.get_id() == id;});

        }

        NodeCollection<Node>::const_iterator find_by_id(element_ID id) const {

            return std::find_if(container.begin(), container.end(),
                                [id](const Node& ele) {return ele.get_id() == id;});

        }

        void remove_by_id(element_ID id) {
            auto it = find_by_id(id);
            
            if (it != container.end()) {
                container.erase(it);
            }

        }
        
        void add(Node&& node){
            container.push_back(std::move(node));
        }

    private:
        container_t container;
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