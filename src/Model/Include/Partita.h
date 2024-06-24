#ifndef CGPRJ2023_24_PARTITA_H
#define CGPRJ2023_24_PARTITA_H

#include <vector>

#include "Player.h"
#include "Enemy.h"
#include "Tiles.h"
#include "PlaneBuilder.hpp"


#define MAPDIM 24

enum GAMESTATE{
    GAMING,
    PAUSE,
    END
};

enum STAGE{
    BIPLANI,
    CACCIA,
    JET
};

class Partita {

    friend class Project;

    int state;
    int stage;
    int killCounter;
    bool bossSpawned = false;

    Player* player;
    std::vector<Plane*> enemies;

    Tiles* map[MAPDIM][MAPDIM];

    void generateWorld();

    Plane* spawn();

    void changeLevel();

    void checkCollision();

    void reset();

    void set_counter();

    void cleanup();

public:
    Partita();

    [[nodiscard]] const Tiles* getMap(int x, int y) const;

     Player *const getPlayer();
};


#endif //CGPRJ2023_24_PARTITA_H
