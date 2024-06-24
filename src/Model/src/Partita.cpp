//
// Created by feder34 on 21/05/24.
//

#include <ctime>
#include <set>
#include <cstdio>
#include "../Include/Partita.h"

#define SKYSCRAPER_RADIUS 4

struct values{
    bool zero;
    bool one;
    bool two;
    bool three;
};

Partita::Partita() {
    this->stage = BIPLANI;
    this->player = new Player();
    this->enemies.clear();
    this->killCounter = 0;
    state = GAMING;
    for(int i = 0; i < MAPDIM; i++)
    {
        for(int j = 0; j < MAPDIM; j++)
        {
            map[i][j] = new Tiles();
        }
    }
}

/**
 * Function generateWorld generates a world using a modified version of the WFC algorithm. Every tile starts with
 * all possible choices {FLOOR, HOUSE, SKYSCRAPER}. They then get randomly selected and the neighboring tiles get updated with the new info.
 * A SKYSCRAPER may only have as neighbours FLOORS and HOUSES.
 * HOUSES AND FLOORS may have anything as a neighbor.
 */
void Partita::generateWorld() {

    std::set<int> choices = {FLOOR, HOUSE, SKYSCRAPER};
    std::set<int> possibleChoices[MAPDIM][MAPDIM];

    for(int i = 0; i < MAPDIM; i++)
    {
        for(int j = 0; j < MAPDIM; j++)
        {
            possibleChoices[i][j] = choices;
        }
    }


    for(int x = 0; x < MAPDIM; x++)
    {
        for(int y = 0; y < MAPDIM; y++)
        {
            if(!possibleChoices[x][y].empty())
            {
                map[x][y]->height = rand() % 3;
                possibleChoices[x][y].clear();

                if (map[x][y]->height == SKYSCRAPER)
                {
                    for (int i = x - SKYSCRAPER_RADIUS; i <= x + SKYSCRAPER_RADIUS; i++)
                    {
                        for (int j = y - SKYSCRAPER_RADIUS; j <= y + SKYSCRAPER_RADIUS; j++)
                        {
                            if (!(i == x && j == y) && i >= 0 && i < MAPDIM && j >= 0 && j < MAPDIM && !possibleChoices[i][j].empty())
                            {
                                possibleChoices[i][j].erase(SKYSCRAPER);
                                int randomNumber = rand() % possibleChoices[i][j].size();
                                auto it = std::begin(possibleChoices[i][j]);

                                std::advance(it, randomNumber);

                                map[i][j]->height = *it;

                                possibleChoices[i][j].clear();
                            }
                        }
                    }
                }
            }
        }
    }

    for(int x = 0; x < MAPDIM; x++)
    {
        for (int y = 0; y < MAPDIM; y++)
        {
            printf("%d ", map[x][y]->height);
        }
        printf("\n");
    }
}

glm::vec3 randomPos()
{
    float max_x = 2.8072f * (MAPDIM-1);
    float max_z = 2.8072f * (MAPDIM-1);
    float min_x = -2.8072f * (MAPDIM-1);
    float min_z = -2.8072f * (MAPDIM-1);

    float x = fmod(rand(),(max_x-min_x + 1)) + min_x;
    float z = fmod(rand(),(max_z-min_z + 1)) + min_z;

    int temp = rand()%4;

    switch(temp)
    {
        case 0:
            return glm::vec3(max_x, 8.40f, z);
            break;
        case 1:
            return glm::vec3(min_x, 8.40f, z);
            break;
        case 2:
            return glm::vec3(x, 8.40f, max_z);
            break;
        case 3:
            return glm::vec3(x, 8.40f, min_z);
            break;
    }
}

Plane* Partita::spawn() {
    //TODO RANDOMIZE SPAWN LOGIC
    if(!bossSpawned && killCounter > 10)
    {
        auto pos = glm::vec3(20.0f, 8.40f, 10.0f);
        auto rot = glm::vec3(0.0f);
        auto plane = PlaneBuilder::getPlane(BOSS, {pos, rot});
        enemies.push_back(plane);
        bossSpawned = true;
        return plane;
    }
    else if(killCounter < 10 && enemies.size() < maxEnemies)
    {
        auto pos = randomPos();
        auto rot = glm::vec3(0.0f);
        auto plane = PlaneBuilder::getPlane(ENEMY, {pos, rot});
        enemies.push_back(plane);
        return plane;
    }

}

void Partita::checkCollision() {
    //Check Collision of player with other enemies

    for(Plane* enemy : enemies)
    {
        if(/*player.pos == enemy.pos*/true)
        {
            state = END;
        }
    }

    //check Collision of player with enemy projectiles
    for(Plane* enemy : enemies)
    {
        /*for(Bullet* p : enemy->projectiles)
        {
            if(p.pos == player.pos)
            {
                state = END;
            }
        }*/
    }

    //check collision of player projectiles with enemies
    /*
    for (Bullet *p : player->projectiles)
    {
        for(Enemy* enemy: enemies)
        {
            if(enemy.pos == p.pos)
            {
                enemies.pop(enemy);
            }
        }
    }*/
}

Player *const Partita::getPlayer(){
    return player;
}

const Tiles* Partita::getMap(int x, int y) const {
    return map[x][y];
}
