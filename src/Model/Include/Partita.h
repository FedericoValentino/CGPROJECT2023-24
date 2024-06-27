#ifndef CGPRJ2023_24_PARTITA_H
#define CGPRJ2023_24_PARTITA_H

#include <vector>

#include "Player.h"
#include "Enemy.h"
#include "Tiles.h"
#include "PlaneBuilder.hpp"


#define MAPDIM 25

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
    static constexpr int MAX_PLANE = 0; // maximum number of planes before boss

    int state;
    int stage;
    int killCounter;
    bool bossSpawned = false;
    const int maxEnemies = 10;
    const int spawnRate = 5;

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
