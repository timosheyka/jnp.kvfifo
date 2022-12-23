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
    using list_ptr_t = typename std::list<std::pair<K, V>>::iterator;
    using map_t = std::map<K, std::list<list_ptr_t>>;
    using list_t = std::list<std::pair<K, V>>;

    std::shared_ptr<map_t> map_list;
    std::shared_ptr<list_t> pair_list;

public:
    kvfifo() :
        map_list(std::make_shared<map_t>()),
        pair_list(std::make_shared<list_t>()) {}
    kvfifo(kvfifo const &other) :
        map_list(other.map_list),
        pair_list(other.pair_list) {}
    kvfifo(kvfifo const &&other)
    { map_list.swap(other.map_list); pair_list.swap(other.pair_list); }

    // O(1) + destroying
    kvfifo& operator=(kvfifo other) {}

    // O(log n)
    void push(K const &k, V const &v) {}

    void pop() {}
    void pop(K const &other) {}

    // O(m + log n)
    void move_to_back(K const &k) {}

    // O(1)
    std::pair<K const &, V &> front() {}
    std::pair<K const &, V &> front() const {}
    std::pair<K const &, V &> back() {}
    std::pair<K const &, V const &> back() const {}

    // O(log n)
    std::pair<K const &, V &> first(K const &key) {}
    std::pair<K const &, V const &> first(K const &key) const {}
    std::pair<K const &, V &> last(K const &key) {}
    std::pair<K const &, V const &> last(K const &key) const {}

    // O(1)
    size_t size() const noexcept { return pair_list->size(); }

    bool empty() const noexcept { return pair_list->empty(); }

    // O(log n)
    size_t count(K const &k) {
        if (map_list->contains(k)) return map_list->find(k)->second.size();
        return 0;
    }

    // O(n)
    void clear() noexcept { map_list->clear(); pair_list->clear(); }; // TODO: weak

    // class Iterator
    class k_iterator {
    private:
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
        { return it = other.it; }
        bool operator!=(const k_iterator &other) const noexcept
        { return !this->operator==(other); }
    };

    k_iterator k_begin() const noexcept { return {map_list->begin()}; }
    k_iterator k_end() const noexcept { return {map_list->end()}; }
};

#endif // KVFIFO_H