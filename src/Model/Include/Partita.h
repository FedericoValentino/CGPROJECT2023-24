#ifndef CGPRJ2023_24_PARTITA_H
#define CGPRJ2023_24_PARTITA_H

#include <vector>

#include "Player.h"
#include "Enemy.h"
#include "Tiles.h"


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
    int state;
    int stage;
    Player* player;
    std::vector<Enemy*> enemies;
    Tiles map[MAPDIM][MAPDIM];


    Partita();

    void generateWorld();

    void spawn();

    void changeLevel();

    void checkCollision();

    void reset();

    void set_counter();

    void cleanup();
};


#endif //CGPRJ2023_24_PARTITA_H
