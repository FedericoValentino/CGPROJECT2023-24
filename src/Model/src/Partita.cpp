//
// Created by feder34 on 21/05/24.
//

#include <ctime>
#include <set>
#include <cstdio>
#include "../Include/Partita.h"

#define SKYSCRAPER_RADIUS 2

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
/*
    for(int x = 0; x < MAPDIM; x++)
    {
        for (int y = 0; y < MAPDIM; y++)
        {
            printf("%d ", map[x][y]->height);
        }
        printf("\n");
    }
*/
}


void Partita::spawn() {
    //enemies.push_back(new Enemy());
}

void Partita::checkCollision() {
    //Check Collision of player with other enemies

    for(Enemy* enemy : enemies)
    {
        if(/*player.pos == enemy.pos*/true)
        {
            state = END;
        }
    }

    //check Collision of player with enemy projectiles
    for(Enemy* enemy : enemies)
    {
        /*for(Projectile* p : enemy->projectiles)
        {
            if(p.pos == player.pos)
            {
                state = END;
            }
        }*/
    }

    //check collision of player projectiles with enemies
    /*
    for (Projectile *p : player->projectiles)
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