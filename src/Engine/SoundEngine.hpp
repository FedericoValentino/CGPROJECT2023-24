#ifndef CGPRJ2023_24_SOUNDENGINE_HPP
#define CGPRJ2023_24_SOUNDENGINE_HPP
#include "../headers/soloud/include/soloud.h"
#include "../headers/soloud/include/soloud_wav.h"

struct sound
{
    SoLoud::Wav soundSample;
    double length;
};

class Sound
{
    friend class Project;

    SoLoud::Soloud gSoloud; // SoLoud engine
    sound intro;
    sound comicalExplosion;
    sound zeppelinExploding;
    sound victory;
    sound playerGun;
    sound enemyGun;
    sound zeppelinGun;
    sound bulletImpact1;
    sound bulletImpact2;
    sound gameOver;
    sound playerExplosion;

public:
    void init() {
        gSoloud.init(); // Initialize SoLoud
        intro.soundSample.load("../src/Sounds/Game-Start.wav");
        comicalExplosion.soundSample.load("../src/Sounds/ComicalExplosion.wav");
        zeppelinExploding.soundSample.load("../src/Sounds/ZeppExplosion1.wav");
        victory.soundSample.load("../src/Sounds/Victory1.wav");
        playerGun.soundSample.load("../src/Sounds/playerGun.wav");
        enemyGun.soundSample.load("../src/Sounds/enemyGun.wav");
        zeppelinGun.soundSample.load("../src/Sounds/laserZap.wav");
        bulletImpact1.soundSample.load("../src/Sounds/farExplosion.wav");
        bulletImpact2.soundSample.load("../src/Sounds/farExplosion2.wav");
        gameOver.soundSample.load("../src/Sounds/womp-womp.wav");
        playerExplosion.soundSample.load("../src/Sounds/explosion1.wav");

        intro.length = intro.soundSample.getLength();
        comicalExplosion.length = comicalExplosion.soundSample.getLength();
        zeppelinExploding.length = zeppelinExploding.soundSample.getLength();
        victory.length = victory.soundSample.getLength();
        playerGun.length = playerGun.soundSample.getLength();
        enemyGun.length = enemyGun.soundSample.getLength();
        zeppelinGun.length = zeppelinGun.soundSample.getLength();
        bulletImpact1.length = bulletImpact1.soundSample.getLength();
        bulletImpact2.length = bulletImpact2.soundSample.getLength();
        gameOver.length = gameOver.soundSample.getLength();
        playerExplosion.length = playerExplosion.soundSample.getLength();

    }

    void playIntro()
    {
        gSoloud.play(intro.soundSample); // Play the wave
    }

    void playComicalExplosion()
    {
        int handle = gSoloud.play(comicalExplosion.soundSample);
        int random = rand()%30 - 1;
        if(random != 5)
            gSoloud.setVolume(handle, 0.5f);

        else
        {
            gSoloud.setVolume(handle, 2.0f);
            gSoloud.setRelativePlaySpeed(handle, 0.3f);
        }

    }

    void playPlayerExplosion()
    {
        gSoloud.play(playerExplosion.soundSample);
    }

    void playZeppelinExpl()
    {
        gSoloud.play(zeppelinExploding.soundSample);
    }

    void playVictory()
    {
        int hande = gSoloud.play(victory.soundSample);
        gSoloud.setVolume(hande, 1.5f);
    }

    void playGameOver()
    {
        gSoloud.play(gameOver.soundSample);
    }

    void playPlayerGun()
    {
        int handle = gSoloud.play(playerGun.soundSample);
        gSoloud.setVolume(handle, 0.7);
    }

    void playEnemyGun()
    {
        gSoloud.play(enemyGun.soundSample);
    }

    void playZeppelinGun()
    {
        int handle = gSoloud.play(zeppelinGun.soundSample);
        gSoloud.setVolume(handle, 0.6);
    }

    void playbulletImpact()
    {
        int random = rand();
        if(random % 2 == 0) {
            int handle1 = gSoloud.play(bulletImpact1.soundSample);
            gSoloud.setVolume(handle1, 0.5);
            gSoloud.setRelativePlaySpeed(handle1, 1.3f);
        }
        else {
            int handle2 = gSoloud.play(bulletImpact2.soundSample);
            gSoloud.setVolume(handle2, 0.5);
            gSoloud.setRelativePlaySpeed(handle2, 1.3f);
        }
    }

    static double getSoundLength(const sound& s)
    {
        return s.length;
    }

    ~Sound()
    {
        gSoloud.deinit();
    }
};

#endif //CGPRJ2023_24_SOUNDENGINE_HPP
