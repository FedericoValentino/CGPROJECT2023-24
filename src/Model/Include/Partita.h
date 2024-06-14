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

    friend class Project;

    int state;
    int stage;
    Player* player;
    std::vector<Enemy*> enemies;
    Tiles* map[MAPDIM][MAPDIM];

    void generateWorld();

    void spawn();

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
