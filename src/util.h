#pragma once

#include <list>
#include <cstdint>

template<typename T>
auto getIterator(T& x)
{
    using Iterator = typename std::list<T>::iterator;
    std::size_t offset = reinterpret_cast<std::size_t>(&(*static_cast<Iterator>(nullptr)));
    Iterator it;
    *reinterpret_cast<std::intptr_t*>(&it) = reinterpret_cast<std::intptr_t>(&x) - offset;
    return it;
}
