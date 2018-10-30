#include "Node.h"
#include <iostream>

Node::Node(const VoronoiDiagram::Site* site, Node* parent) : 
    parent(parent), left(nullptr), right(nullptr), 
    site(site), leftHalfEdge(nullptr), rightHalfEdge(nullptr), event(nullptr),
    prev(nullptr), next(nullptr) 
{

}

bool Node::isLeaf() const
{
    return left == nullptr && right == nullptr;
}
void Node::display(std::string tabs) const
{
    //std::cout << tabs << site->index << ' ' /*<< this << " (parent: " << parent << ", prev: " << prev << ", next: " << next << ") -> " << left << " " << right << std::endl*/;
    std::cout << leftHalfEdge << ' ' << rightHalfEdge << std::endl;
    if (left != nullptr)
        left->display(tabs + '\t');
    if (right != nullptr)
        right->display(tabs + '\t');
}
