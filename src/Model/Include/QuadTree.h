#ifndef CGPRJ2023_24_QUADTREE_H
#define CGPRJ2023_24_QUADTREE_H

#include <vector>
#include "Plane.h"
#include "Bullet.h"

#define MAXCOUNT 10



class QuadTree
{
    std::vector<Plane*> planes;
    std::vector<Bullet*> bullets;

    int playerIndex = -1;

    int minX;
    int minZ;
    int maxX;
    int maxZ;

    QuadTree* NE;
    QuadTree* NW;
    QuadTree* SE;
    QuadTree* SW;

private:
    void collisionCheck();

public:
    QuadTree(int minX, int minZ, int maxX, int maxZ);

    void insert(Plane* p);

    void insert(Bullet* b);

    void checkCollision();
};


#endif //CGPRJ2023_24_QUADTREE_H
