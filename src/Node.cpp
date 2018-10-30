#include "Node.h"
#include <iostream>

void Node::display(std::string tabs) const
{
    //std::cout << tabs << site->index << ' ' /*<< this << " (parent: " << parent << ", prev: " << prev << ", next: " << next << ") -> " << left << " " << right << std::endl*/;
    std::cout << leftHalfEdge << ' ' << rightHalfEdge << std::endl;
    if (left != nullptr)
        left->display(tabs + '\t');
    if (right != nullptr)
        right->display(tabs + '\t');
}
