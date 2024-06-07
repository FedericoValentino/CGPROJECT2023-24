#include <chrono>
#include "../src/Model/Include/Enemy.h"
#include "../src/Model/Include/Position3D.h"
#include "../src/Model/Include/Bullet.h"




int main() {
    constexpr float SCREEN_WIDHT = 800.0f;
    constexpr float SCREEN_HEIGHT = 600.0f;
    const glm::vec3 origin = {0.1f,0.1f,0.0f};
    const glm::vec3 orientation = {glm::radians(45.0f),glm::radians(45.0f),0.0f};
    const Position3D pos(origin,orientation);
    Enemy enemy(pos,10.0f,true);
    Player player;
    enemy.shoot(10.0f,SCREEN_WIDHT,SCREEN_HEIGHT);
    enemy.shoot(10.0f,SCREEN_WIDHT,SCREEN_HEIGHT);
    enemy.shoot(10.0f,SCREEN_WIDHT,SCREEN_HEIGHT);
    enemy.shoot(10.0f,SCREEN_WIDHT,SCREEN_HEIGHT);
    enemy.shoot(10.0f,SCREEN_WIDHT,SCREEN_HEIGHT);

    for(int i =0;i<10;++i)
    {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        enemy.move(player,10.0f);
    }
    return 0;

    // Enemy can perform other actions here
}