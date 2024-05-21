//
// Created by feder34 on 21/05/24.
//

#include "../Include/Partita.h"

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

}


void Partita::generateWorld() {


}


void Partita::spawn() {
    enemies.push_back(new Enemy());
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