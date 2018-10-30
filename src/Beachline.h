#pragma once

#include "Vector2.h"
#include "VoronoiDiagram.h"

class Node;

class Beachline
{
public:
    Beachline();
    ~Beachline();

    Node* createNode(const VoronoiDiagram::Site* site);
    
    bool isEmpty() const;
    bool isNil(Node* x) const;
    void setRoot(Node* root);

    Node* locateArcAbove(Vector2f point, float l) const;
    void insertBefore(Node* x, Node* y);
    void insertAfter(Node* x, Node* y);
    void replaceNode(Node* oldNode, Node* newNode);
    void remove(Node* z);

    void display() const;

private:
    Node* mNil;
    Node* mRoot;

    // Utility methods
    Node* minimum(Node* x) const;
    void transplant(Node* u, Node* v); 

    // Fixup functions
    void insertFixup(Node* z);
    void removeFixup(Node* x);

    float computeBreakpoint(Vector2f point1, Vector2f point2, float l) const;

    void free(Node* node);
};
