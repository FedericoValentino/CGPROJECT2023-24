#ifndef CGPRJ2023_24_PARTITA_H
#define CGPRJ2023_24_PARTITA_H

#include <vector>

#include "Player.h"
#include "Enemy.h"
#include "Tiles.h"
#include "../lib/PerlinNoise.hpp"


#define MAPDIM 1000

enum STAGE{
    BIPLANI,
    CACCIA,
    JET
};

class Partita {
    int stage;
    Player player;
    std::vector<Enemy> enemies;
    std::vector<Tiles> map;

    siv::PerlinNoise::seed_type seed = 123456u;
    siv::PerlinNoise perlin{seed};



    void generateWorld();

    void spawn();

    void changeLevel();

    void checkCollision();

    void reset();

    void set_counter();

    void cleanup();
};


#endif //CGPRJ2023_24_PARTITA_H
