//
// Created by feder34 on 21/05/24.
//

#include <ctime>
#include <set>
#include <cstdio>
#include "../Include/Partita.h"

#define SKYSCRAPER_RADIUS 6


Partita::Partita() {
    this->stage = BIPLANI;
    this->player = std::make_shared<Player>();
    this->enemies.clear();
    this->killCounter = 0;
    state = GAMING;
    for(int i = 0; i < MAPDIM; i++)
    {
        for(int j = 0; j < MAPDIM; j++)
        {
            map[i][j] = std::make_shared<Tiles>(i,j);
        }
    }
    srand(time(nullptr));
    this->skyscrapers.clear();
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

    //Creates the Map
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
                    skyscrapers.insert(map[x][y]);
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

//TODO change later to killCounter >= MAX_PLANE
std::shared_ptr<Plane> Partita::spawn() {
    if(!bossSpawned && killCounter == 0)
    {
        auto pos = randomPos();
        auto rot = glm::vec3(0.0f);
        auto plane = PlaneBuilder::build(BOSS, {pos, rot});
        enemies.insert(plane);
        bossSpawned = true;
        return plane;
    }
    //TODO change later to < MAX_PLANE
    else if(killCounter ==  1)
    {
        auto pos = randomPos();
        auto rot = glm::vec3(0.0f);
        auto plane = PlaneBuilder::build(ENEMY, {pos, rot});
        enemies.insert(plane);
        //killCounter++;
        return plane;
    }
    return nullptr;
}

void Partita::checkCollision(float deltaT) {

    //RIEMPIO ALBERO O(N)

    //SCORRO L'ALBERO

    //TODO Remove comments
    static constexpr float RADIUS_COLLISION = 5.5f;
    //Check collision player with skyscrapers.
    /*for(auto skyscraper : skyscrapers)
        if(skyscraper->checkCollision(player->getPosition().origin.x,player->getPosition().origin.z,RADIUS_COLLISION))
            state=END;*/

    //Check Collision of player with other enemies and bosses
    float radius = 4 * (MAPDIM);
    for(auto enemy : enemies)
        if(player->checkDistance3D(enemy->getPosition().origin, player->getPosition().origin, PLAYER) && !enemy->getDead())
            state = END;


    //Check collision of Enemy with Skyscraper;
    for(auto enemy : enemies)
        for(auto skyscraper : skyscrapers)
        {
            if(enemy->getType() == ENEMY) {
                if (skyscraper->checkCollision(enemy->getPosition().origin.x, enemy->getPosition().origin.z, 11.0f) && enemy->getPosition().origin.y <= 10.0f)
                    enemy->setAvoidBuilding(true);
            }
            else
            {
                if (skyscraper->checkCollision(enemy->getPosition().origin.x, enemy->getPosition().origin.z, 11.0f))
                    enemy->setAvoidBuilding(true);
            }
        }


    //check Collision of player with enemy projectiles
    for(auto enemy : enemies)
    {
        if(!enemy->getDead())
        {
            for (auto bullet: enemy->getBullets()) {
                float dist = glm::distance(glm::vec3(0.0f, 0.0f, 0.0f), bullet->getPosition3D().origin);
                if (player->checkDistance3D(player->getPosition().origin, bullet->getPosition3D().origin, PLAYER)) {
                    player->planeHit(*bullet);
                    enemy->clearBullet(bullet);
                    if (player->getDead()) {
                        state = END;
                    }
                }
                else if(dist > radius)
                {
                    enemy->clearBullet(bullet);
                }
                for(auto skyscraper : skyscrapers)
                {
                    if(skyscraper->checkCollision(bullet->getPosition3D().origin.x, bullet->getPosition3D().origin.z,RADIUS_COLLISION) && bullet->getPosition3D().origin.y <= 11.40f)
                        enemy->clearBullet(bullet);
                }
            }
        }
    }


    //check collision of player projectiles with enemies
    std::vector<std::shared_ptr<Plane>> toDelete;
    for (auto p : player->getBullets())
    {
        float dist = glm::distance(glm::vec3(0.0f, 0.0f, 0.0f), p->getPosition3D().origin);
        for(auto enemy: enemies)
        {

            if(enemy->checkDistance3D( enemy->getPosition().origin, p->getPosition3D().origin, PLAYER) && !enemy->getDead())
            {
                enemy->planeHit(*p);
                player->clearBullet(p);
                if(enemy->getDead()) {
                    toDelete.push_back(enemy);
                    killCounter++;
                }
            }
        }
        if(dist > radius)
        {
            player->clearBullet(p);
        }
        for(auto skyscraper : skyscrapers)
        {
            if(skyscraper->checkCollision(p->getPosition3D().origin.x, p->getPosition3D().origin.z,RADIUS_COLLISION))
                player->clearBullet(p);
        }
    }
    for(auto enemy : toDelete)
        enemies.erase(enemy);
}

std::shared_ptr<const Player> Partita::getPlayer() const{
    return player;
}

std::shared_ptr<const Tiles> Partita::getMap(int x, int y) const {
    return map[x][y];
}
