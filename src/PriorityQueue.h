/* FortuneAlgorithm
 * Copyright (C) 2018 Pierre Vigier
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

// TO DO: Replace raw pointers by std::unique_ptr

#pragma once

// STL
#include <ostream>
#include <vector>
#include <memory>

template<typename T>
class PriorityQueue
{
public:
    PriorityQueue()
    {

    }

    // Accessors

    bool isEmpty() const
    {
        return mElements.empty();
    }

    // Operations

    std::unique_ptr<T> pop()
    {
        swap(0, mElements.size() - 1);
        auto top = std::move(mElements.back());
        mElements.pop_back();
        siftDown(0);
        return top;
    }

    void push(std::unique_ptr<T> elem)
    {
        elem->index = mElements.size();
        mElements.emplace_back(std::move(elem));
        siftUp(mElements.size() - 1);
    }

    void update(std::size_t i)
    {
        int parent = getParent(i);
        if(parent >= 0 && *mElements[parent] < *mElements[i])
            siftUp(i);
        else
            siftDown(i);
    }

    void remove(std::size_t i)
    {
        swap(i, mElements.size() - 1);
        mElements.pop_back();
        if (i < mElements.size())
            update(i);
    }

    // Print 

    std::ostream& print(std::ostream& os, int i = 0, std::string tabs = "") const
    {
        if(i < mElements.size())
        {
            os << tabs << *mElements[i] << std::endl;
            display(getLeftChild(i), tabs + '\t');
            display(getRightChild(i), tabs + '\t');
        }
        return os;
    }

private:
    std::vector<std::unique_ptr<T>> mElements;

    // Accessors

    int getParent(int i) const
    {
        return (i + 1) / 2 - 1;
    }

    std::size_t getLeftChild(std::size_t i) const
    {
        return 2 * (i + 1) - 1;
    }

    std::size_t getRightChild(std::size_t i) const
    {
        return 2 * (i + 1);
    }

    // Operations

    void siftDown(std::size_t i)
    {
        std::size_t left = getLeftChild(i);
        std::size_t right = getRightChild(i);
        std::size_t j = i;
        if(left < mElements.size() && *mElements[j] < *mElements[left])
            j = left;
        if(right < mElements.size() && *mElements[j] < *mElements[right])
            j = right;
        if(j != i)
        {
            swap(i, j);
            siftDown(j);
        }
    }

    void siftUp(std::size_t i)
    {
        int parent = getParent(i);
        if(parent >= 0 && *mElements[parent] < *mElements[i])
        {
            swap(i, parent);
            siftUp(parent);
        }
    }

    inline void swap(std::size_t i, std::size_t j)
    {
        auto tmp = std::move(mElements[i]);
        mElements[i] = std::move(mElements[j]);
        mElements[j] = std::move(tmp);
        mElements[i]->index = i;
        mElements[j]->index = j;
    }
};

template <typename T>
std::ostream& operator<<(std::ostream& os, const PriorityQueue<T>& queue)
{
    return queue.print(os);
}
