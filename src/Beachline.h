#pragma once

#include "Vector2.h"
#include "VoronoiDiagram.h"

class Arc;

class Beachline
{
public:
    Beachline();
    ~Beachline();

    Arc* createArc(const VoronoiDiagram::Site* site);
    
    bool isEmpty() const;
    bool isNil(Arc* x) const;
    void setRoot(Arc* x);

    Arc* locateArcAbove(Vector2f point, float l) const;
    void insertBefore(Arc* x, Arc* y);
    void insertAfter(Arc* x, Arc* y);
    void replace(Arc* x, Arc* y);
    void remove(Arc* z);

    void display() const;

private:
    Arc* mNil;
    Arc* mRoot;

    // Utility methods
    Arc* minimum(Arc* x) const;
    void transplant(Arc* u, Arc* v); 

    // Fixup functions
    void insertFixup(Arc* z);
    void removeFixup(Arc* x);

    // Rotations
    void leftRotate(Arc* x);
    void rightRotate(Arc* y);

    float computeBreakpoint(Vector2f point1, Vector2f point2, float l) const;

    void free(Arc* x);
};
