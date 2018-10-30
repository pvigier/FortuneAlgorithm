#pragma once

#include "Vector2.h"

class Node;

class Beachline
{
public:
    Beachline();
    ~Beachline();

    bool isEmpty();
    Node* getRoot();
    void setRoot(Node* root);

    Node* locateArcAbove(Vector2f point, float l) const;
    void insertBefore(Node* x, Node* y);
    void insertAfter(Node* x, Node* y);
    void replaceNode(Node* oldNode, Node* newNode);
    void remove(Node* z);
    Node* minimum(Node* x);

    void display();

private:
    Node* mRoot;

    void transplant(Node* u, Node* v); 

    float computeBreakpoint(Vector2f point1, Vector2f point2, float l) const;
};
