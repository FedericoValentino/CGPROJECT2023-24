#ifndef CGPRJ2023_24_PARTITA_H
#define CGPRJ2023_24_PARTITA_H

#include <vector>
#include <array>

#include "Player.h"
#include "Enemy.h"
#include "Tiles.h"
#include "PlaneBuilder.hpp"
#include "Constants.h"

#define MAX_ENEMIES_IN_MAP 20


enum GAMESTATE{
    GAMING,
    END
};

enum STAGE{
    BIPLANI,
    CACCIA,
    JET
};

class Partita {
    using mapStructure = std::array<std::array<std::shared_ptr<Tiles>,constant::MAPDIM>,constant::MAPDIM>;
    friend class Project;
    int stage;
    int state;
    int killCounter;
    bool bossSpawned = false;
    bool bossDead = false;
    const int spawnRate = 5;

    std::shared_ptr<Player> player;
    std::set<std::shared_ptr<Plane>> enemies;

    mapStructure map;
    std::set<std::shared_ptr<Tiles>> skyscrapers;

    void generateWorld();

    std::shared_ptr<Plane> spawn();

    void changeLevel();

    void checkCollision(float d);

    void reset();

    void set_counter();

    void cleanup();

public:

    static constexpr int MAX_PLANE = 1; // maximum number of planes before boss

    Partita();

    [[nodiscard]] std::shared_ptr<const Tiles> getMap(int x, int y) const;

    std::shared_ptr<const Player> getPlayer() const;
};


#endif //CGPRJ2023_24_PARTITA_H
