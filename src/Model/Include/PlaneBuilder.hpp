
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
    static std::shared_ptr<Plane> build(PLANETYPE_UTILITY type, const Position3D& position){

        switch(type) {
            case PLAYER:
                return std::make_shared<Player>();
            case ENEMY:
                return std::make_shared<Enemy>(position);
            case BOSS:
                return std::make_shared<Boss>(position);
            default:
                return nullptr;
        }
    }
};