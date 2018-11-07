#pragma once

#include "Vector2.h"
#include "VoronoiDiagram.h"

class Arc;

class Beachline
{
public:
    Beachline();
    ~Beachline();

    // Remove copy and move operations
    Beachline(const Beachline&) = delete;
    Beachline& operator=(const Beachline&) = delete;
    Beachline(Beachline&&) = delete;
    Beachline& operator=(Beachline&&) = delete;

    Arc* createArc(const VoronoiDiagram::Site* site);
    
    bool isEmpty() const;
    bool isNil(const Arc* x) const;
    void setRoot(Arc* x);
    Arc* getLeftmostArc() const;

    Arc* locateArcAbove(Vector2 point, double l) const;
    void insertBefore(Arc* x, Arc* y);
    void insertAfter(Arc* x, Arc* y);
    void replace(Arc* x, Arc* y);
    void remove(Arc* z);

    std::ostream& print(std::ostream& os) const;

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

    double computeBreakpoint(Vector2 point1, Vector2 point2, double l) const;

    void free(Arc* x);

    std::ostream& printArc(std::ostream& os, const Arc* arc, std::string tabs = "") const;
};

std::ostream& operator<<(std::ostream& os, const Beachline& beachline);
