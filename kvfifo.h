#ifndef KVFIFO_H
#define KVFIFO_H

#include <cstddef>
#include <iostream>
#include <iterator>
#include <list>
#include <map>
#include <memory>
#include <set>
#include <utility>
#include <vector>

template <typename K, typename V> class kvfifo {
private:
    using list_t = std::list<std::pair<K const, V>>;
    using list_ptr_t = typename list_t::iterator;
    using map_t = std::map<K, std::list<list_ptr_t>>;

    std::shared_ptr<map_t> map_list;
    std::shared_ptr<list_t> pair_list;
    bool must_be_uniqe = false;

    std::pair<K const &, V &> make_ref(std::pair<K const , V > &p){
        return std::pair<K const &, V &> ({std::ref(p.first),  std::ref(p.second)});
    }

    void make_unique(){
        if(!map_list.unique()){
            auto original_pair_list = pair_list.get();
            pair_list = std::make_shared<list_t>(*pair_list);

            try {
                map_t new_map_lists;
                for (auto it = pair_list->begin(); it != pair_list->end(); ++it) {
                    new_map_lists[it->first].push_back(it);
                }

                map_list = std::make_shared<map_t>(new_map_lists);
            } catch (std::exception& e) {
                pair_list.reset(original_pair_list);
                throw;
            }
        }
    }
public:
    kvfifo() :
        map_list(std::make_shared<map_t>()),
        pair_list(std::make_shared<list_t>()) {}
    kvfifo(kvfifo const &other) {
            if(other.must_be_uniqe){
                pair_list = std::make_shared<list_t>(*other.pair_list);

                map_t new_map_list;
                for (auto it = pair_list->begin(); it != pair_list->end(); ++it) {
                    new_map_list[it->first].push_back(it);
                }

                map_list = std::make_shared<map_t>(new_map_list);
            } else{
                map_list = other.map_list;
                pair_list = other.pair_list;
            }
        }
    kvfifo(kvfifo &&other) noexcept : map_list(std::move(other.map_list)), pair_list(std::move(other.pair_list)),
        must_be_uniqe(std::move(other.must_be_uniqe)) {}

    // O(1) + destroying
    kvfifo& operator=(kvfifo other) {
        map_list = (other.map_list);
        pair_list = (other.pair_list);
        must_be_uniqe = other.must_be_uniqe;
        return *this;
    }

    // O(log n)
    void push(K const &k, V const &v) {
        make_unique();
        
        pair_list->push_back( std::make_pair(k, v));

        try {
            if(!map_list->contains(k)){
                std::list<list_ptr_t> new_l;
                map_list->insert({k, new_l});
            }
            map_list->find(k)->second.push_back(
            --pair_list->end()
            );
        } catch (std::exception& e) {
            pair_list->pop_back();
            throw;
        }

        must_be_uniqe = false;
    }

    void pop() {
        if(pair_list->empty()) throw std::invalid_argument("queue is empty");
        make_unique();

        K k = pair_list->front().first;
        map_list->find(k)->second.pop_front();
        if(map_list->find(k)->second.size()==0){
            map_list->erase(k);
        }
        pair_list->pop_front();
        must_be_uniqe = false;
    }
    void pop(K const &k) {
        if(!map_list->contains(k)) throw std::invalid_argument("key doesn't exist");
        make_unique();


        auto first = map_list->find(k)->second.front();
        std::cout<<"mk\n";
        pair_list->erase(first);
        std::cout<<"mk\n";
        map_list->find(k)->second.pop_front();
        if(map_list->find(k)->second.size() == 0){
            map_list->erase(k);
        }
        must_be_uniqe = false;
    }

    // O(m + log n)
    void move_to_back(K const &k) {
        if(!map_list->contains(k)) throw std::invalid_argument("key doesn't exist");
        make_unique();

        std::list<list_ptr_t> to_move;
        std::swap(to_move, map_list->find(k)->second);

        try{
            list_t cells_to_move;
            for(list_ptr_t ptr : to_move){
                std::pair<K const ,V> cell = *ptr;
                cells_to_move.push_back(cell);
                pair_list->erase(ptr);
            }
            for(auto cell : cells_to_move){
                push(cell.first, cell.second);
            }


        }  catch (std::exception& e) {
            throw;
        }

        must_be_uniqe = false;
        
    }

    // O(1)
    
   
    std::pair<K const &, V const &> front() const {
        if(pair_list->empty()) throw std::invalid_argument("queue is empty");
        // std::cout<<"f1\n";

        return pair_list->front();
    }

    std::pair<K const &, V &> front() {
        if(pair_list->empty()) throw std::invalid_argument("queue is empty");
        // std::cout<<"f2\n";
        make_unique();
        must_be_uniqe = true;

        return make_ref(pair_list->front()); 
    }
    
    std::pair<K const &, V const &> back() const {
        if(pair_list->empty()) throw std::invalid_argument("queue is empty");

        return pair_list->back();
    }
    std::pair<K const &, V &> back() { 
        if(pair_list->empty()) throw std::invalid_argument("queue is empty");
        make_unique();
        must_be_uniqe = true;

        return make_ref(pair_list->back()); 
     }
    

    // O(log n)
    std::pair<K const &, V const &> first(K const &k) const {
        if(!map_list->contains(k)) throw std::invalid_argument("key doesn't exist");

        return *map_list->find(k)->second.front();
    }
    std::pair<K const &, V &> first(K const &k) { 
        if(!map_list->contains(k)) throw std::invalid_argument("key doesn't exist");
        make_unique();
        must_be_uniqe = true;

        return make_ref(*map_list->find(k)->second.front());
    }
    
    std::pair<K const &, V const &> last(K const &k) const {
        if(!map_list->contains(k)) throw std::invalid_argument("key doesn't exist");


        return *map_list->find(k)->second.back();
    }
    std::pair<K const &, V &> last(K const &k) { 
        if(!map_list->contains(k)) throw std::invalid_argument("key doesn't exist");
        make_unique();
        must_be_uniqe = true;

        return make_ref(*map_list->find(k)->second.back());
    }
    

    // O(1)
    std::size_t size() const noexcept { return pair_list->size(); }

    bool empty() const noexcept { 
        return  pair_list->empty(); }

    // O(log n)
    std::size_t count(K const &k) const noexcept {
        if (map_list->contains(k)) return map_list->find(k)->second.size();
        return 0;
    }

    // O(n)
    void clear() noexcept { 
        make_unique();
        map_list->clear();pair_list->clear();
        must_be_uniqe = false;
    }; 
    // class Iterator
    class k_iterator {
        typename map_t::const_iterator it;
    public:
        using iterator_category = std::bidirectional_iterator_tag;
        using value_type = const K;
        using difference_type = ptrdiff_t;
        using pointer = const K *;
        using reference = const K &;

        k_iterator() = default;
        k_iterator(K it) : it(it) {}
        k_iterator(const k_iterator &other) : it(other.it) {}
        k_iterator(typename map_t::const_iterator &&it) : it(it) {}

        k_iterator operator++() noexcept
        { ++it; return *this; }
        k_iterator operator++(int) noexcept
        { auto tmp = *this; ++(*this); return tmp; }

        k_iterator operator--() noexcept
        { --it; return *this; }
        k_iterator operator--(int) noexcept
        { auto tmp = *this; --(*this); return tmp; }

        reference operator*() const noexcept
        { return (*it).first; }
        pointer operator->() const noexcept
        { return &(it->first); }

        k_iterator &operator=(const k_iterator &other) noexcept = default;
        bool operator==(const k_iterator &other) const noexcept
        { return it == other.it; }
        bool operator!=( k_iterator &other) const noexcept
        { return !this->operator==(other); }
    };

    k_iterator k_begin() const noexcept { return {map_list->begin()}; }
    k_iterator k_end() const noexcept { return {map_list->end()}; }
};

#endif // KVFIFO_H