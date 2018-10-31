#include "Arc.h"

std::ostream& Arc::print(std::ostream& os, std::string tabs) const
{
    os << leftHalfEdge << ' ' << rightHalfEdge << std::endl;
    if (left != nullptr)
        left->print(os, tabs + '\t');
    if (right != nullptr)
        right->print(os, tabs + '\t');
    return os;
}
