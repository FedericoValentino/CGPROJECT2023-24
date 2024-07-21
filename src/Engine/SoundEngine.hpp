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

public:
    void init() {
        gSoloud.init(); // Initialize SoLoud
        intro.soundSample.load("../src/Sounds/Game-Start.wav");
        comicalExplosion.soundSample.load("../src/Sounds/ComicalExplosion.wav");
        zeppelinExploding.soundSample.load("../src/Sounds/ZeppExplosion1.wav");
        victory.soundSample.load("../src/Sounds/Victory1.wav");
        playerGun.soundSample.load("../src/Sounds/playerGun.wav");
        enemyGun.soundSample.load("../src/Sounds/enemyGun.wav");

        intro.length = intro.soundSample.getLength();
        comicalExplosion.length = comicalExplosion.soundSample.getLength();
        zeppelinExploding.length = zeppelinExploding.soundSample.getLength();
        victory.length = victory.soundSample.getLength();
        playerGun.length = playerGun.soundSample.getLength();
        enemyGun.length = enemyGun.soundSample.getLength();

    }

    void playIntro()
    {
        gSoloud.play(intro.soundSample); // Play the wave
    }

    void playComicalExplosion()
    {
        int handle = gSoloud.play(comicalExplosion.soundSample);
        int random = rand()%101 - 1;
        if(random != 5)
            gSoloud.setVolume(handle, 0.5f);
        else
        {
            gSoloud.setVolume(handle, 2.0f);
            gSoloud.setRelativePlaySpeed(handle, 0.3f);
        }

    }

    void playZeppelinExpl()
    {
        gSoloud.play(zeppelinExploding.soundSample);
    }

    void playVictory()
    {
        int hande = gSoloud.play(victory.soundSample);
        gSoloud.setVolume(hande, 2.0f);
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
