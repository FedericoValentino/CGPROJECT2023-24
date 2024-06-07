
#include "Plane.h"
#include "Player.h"
#include "Enemy.h"
#include "Boss.h"


class PlaneBuilder {

public:

    Plane* getPlane(PLANETYPE type, const Position3D& position){

        Plane* plane;
        switch(type){

            case PLAYER:
                plane = new Player();

                break;
            case ENEMY:
                plane = new Enemy(position);

                break;
            case BOSS:
                plane = new Boss(position);

                break;
            default:
                break;
        }

        return plane;
    }


};