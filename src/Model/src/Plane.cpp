//
// Created by Pudduh on 07/06/2024.
//

#include "../Include/Plane.h"

/*Plane::Plane(float speed)
{
    this->position = new Position3D();
    this->speed = speed;

}*/

void Plane::shoot()
{
    bullets.insert(new Bullet(position, 1, 1.0f, false));
}

void Plane::planeHit(Bullet bullet)
{
    if (bullet.getPosition3D().origin == position.origin)
        hp -= bullet.getSize();
}

void Plane::clearBullet(Bullet* own)
{
    bullets.erase(own);
}

void Plane::changePosition() {

}

void Plane::changeDirection() {

}

Position3D Plane::getPosition() {
    return position;
}

const float Plane::getSpeed() {
    return speed;
}

std::set<Bullet*> Plane::getBullets() {
    return bullets;
}

bool Plane::getHit() {
    return hit;
}

int Plane::getHP() {
    return hp;
}
