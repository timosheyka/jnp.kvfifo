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

public:
    kvfifo() {}
    kvfifo(kvfifo const &other) {}
    kvfifo(kvfifo const &&other) {}

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
    size_t size() const {}

    bool empty() const {}

    // O(log n)
    size_t count(K const &k) {}

    // O(n)
    void clear() {};

    // class Iterator
};

#endif // KVFIFO_H