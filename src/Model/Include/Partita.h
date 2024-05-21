#ifndef CGPRJ2023_24_PARTITA_H
#define CGPRJ2023_24_PARTITA_H

#include <vector>

#include "Player.h"
#include "Enemy.h"

enum STAGE{
    BIPLANI,
    CACCIA,
    JET
};

class Partita {
    int stage;
    Player player;
    std::vector<Enemy> enemies;

};


#endif //CGPRJ2023_24_PARTITA_H
