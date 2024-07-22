#include "../Include/QuadTree.h"
#include "../Include/Player.h"

QuadTree::QuadTree(int minX, int minZ, int maxX, int maxZ)
{
    this->minX = minX;
    this->minZ = minZ;
    this->maxX = maxX;
    this->maxZ = maxZ;

    this->NE = nullptr;
    this->SE = nullptr;
    this->NW = nullptr;
    this->SW = nullptr;
}


void QuadTree::insert(Plane *p)
{
    if(this->planes.size() + this->bullets.size() < MAXCOUNT)
    {
        this->planes.push_back(p);
    }
    else
    {
        int halfWidth = (maxX - minX)/2;
        int halfHeight = (maxZ - minZ)/2;
        NW = new QuadTree(minX, minZ, minX + halfWidth, minZ + halfHeight);
        SW = new QuadTree(minX, minZ + halfWidth, minX + halfWidth, maxZ);
        NE = new QuadTree(minX + halfWidth, minZ, maxX, minZ + halfHeight);
        SE = new QuadTree(minX + halfWidth, minZ + halfHeight, maxX, maxZ);

        this->planes.push_back(p);
        if(p->getType() == PLAYER)
        {
            this->playerIndex = this->planes.size()-1;
        }

        for(Plane* plane : this->planes)
        {
            glm::vec3 pos = plane->getPosition().origin;

            bool up = false;
            bool right = false;

            if(pos.x >= halfWidth)
            {
                up = true;
            }
            if(pos.z >= halfHeight)
            {
                right = true;
            }

            if(up)
            {
                if(right)
                    NE->insert(plane);
                else
                    NW->insert(plane);
            }
            else
            {
                if(right)
                    SE->insert(plane);
                else
                    SW->insert(plane);
            }
        }

        for(Bullet* b : this->bullets)
        {
            glm::vec3 pos = b->getPosition3D().origin;

            bool up = false;
            bool right = false;

            if(pos.x >= halfWidth)
            {
                up = true;
            }
            if(pos.z >= halfHeight)
            {
                right = true;
            }

            if(up)
            {
                if(right)
                    NE->insert(b);
                else
                    NW->insert(b);
            }
            else
            {
                if(right)
                    SE->insert(b);
                else
                    SW->insert(b);
            }
        }
    }

}


void QuadTree::insert(Bullet *b)
{
    if(this->bullets.size() + this->bullets.size() < MAXCOUNT)
    {
        this->bullets.push_back(b);
    }
    else
    {
        int halfWidth = (maxX - minX)/2;
        int halfHeight = (maxZ - minZ)/2;
        NW = new QuadTree(minX, minZ, minX + halfWidth, minZ + halfHeight);
        SW = new QuadTree(minX, minZ + halfWidth, minX + halfWidth, maxZ);
        NE = new QuadTree(minX + halfWidth, minZ, maxX, minZ + halfHeight);
        SE = new QuadTree(minX + halfWidth, minZ + halfHeight, maxX, maxZ);

        this->bullets.push_back(b);

        for(Plane* plane : this->planes)
        {
            glm::vec3 pos = plane->getPosition().origin;

            bool up = false;
            bool right = false;

            if(pos.x >= halfWidth)
            {
                up = true;
            }
            if(pos.z >= halfHeight)
            {
                right = true;
            }

            if(up)
            {
                if(right)
                    NE->insert(plane);
                else
                    NW->insert(plane);
            }
            else
            {
                if(right)
                    SE->insert(plane);
                else
                    SW->insert(plane);
            }
        }

        for(Bullet* bullet : this->bullets)
        {
            glm::vec3 pos = b->getPosition3D().origin;

            bool up = false;
            bool right = false;

            if(pos.x >= halfWidth)
            {
                up = true;
            }
            if(pos.z >= halfHeight)
            {
                right = true;
            }

            if(up)
            {
                if(right)
                    NE->insert(bullet);
                else
                    NW->insert(bullet);
            }
            else
            {
                if(right)
                    SE->insert(bullet);
                else
                    SW->insert(bullet);
            }
        }
    }

}

void QuadTree::checkCollision()
{
    if(NE)
        NE->checkCollision();
    if(NW)
        NW->checkCollision();
    collisionCheck();
    if(SE)
        SE->checkCollision();
    if(SW)
        SW->checkCollision();
}

void QuadTree::collisionCheck() {

    //Bullets with planes
    for(Bullet* b : bullets)
    {
        for(Plane* p: planes)
        {
            //No friendly fire
            if(b->getType() != p->getType())
            {
                if(p->checkDistance3D(p->getPosition().origin, b->getPosition3D().origin, PLAYER))
                {
                    //Changing the flag of the bullet to be cleared
                    b->toClear = true;
                    //Hitting the plane
                    p->planeHit(*b);
                }
            }
        }
    }


    //Player with enemy collision
    if(playerIndex != -1)
    {
        Plane* player = planes[playerIndex];
        for(Plane* p: planes)
        {
            if(p != player)
            {
                if(player->checkDistance3D(p->getPosition().origin, player->getPosition().origin, PLAYER) && !p->getDead())
                {
                    dynamic_cast<Player*>(player)->kill();
                }
            }
        }
    }
}
