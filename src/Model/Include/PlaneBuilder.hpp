
#include "Plane.h"
#include "Player.h"
#include "Enemy.h"
#include "Boss.h"

/**
 * A factory class
 */
class PlaneBuilder {

public:

    /**
     * Factory method that calls the right constructor based on the PLANETYPE_UTILITY of the plane we want to create
     * @param type the PLANETYPE_UTILITY
     * @param position the position to assign to some of the planes
     * @return the constructed plane
     */
    static Plane* getPlane(PLANETYPE_UTILITY type, const Position3D& position){

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