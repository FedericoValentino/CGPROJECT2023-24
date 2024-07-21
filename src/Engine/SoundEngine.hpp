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

public:
    void init() {
        gSoloud.init(); // Initialize SoLoud
        intro.soundSample.load("../src/Sounds/Game-Start.wav");
        comicalExplosion.soundSample.load("../src/Sounds/ComicalExplosion.wav");
        zeppelinExploding.soundSample.load("../src/Sounds/ZeppExplosion1.wav");
        victory.soundSample.load("../src/Sounds/Victory1.wav");

        intro.length = intro.soundSample.getLength();
        comicalExplosion.length = comicalExplosion.soundSample.getLength();
        zeppelinExploding.length = zeppelinExploding.soundSample.getLength();
        victory.length = victory.soundSample.getLength();

    }

    void playIntro()
    {
        gSoloud.play(intro.soundSample); // Play the wave
    }

    void playComicalExplosion()
    {
        gSoloud.play(comicalExplosion.soundSample);
    }

    void playZeppelinExpl()
    {
        gSoloud.play(zeppelinExploding.soundSample);
    }

    void playVictory()
    {
        gSoloud.play(victory.soundSample);
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
