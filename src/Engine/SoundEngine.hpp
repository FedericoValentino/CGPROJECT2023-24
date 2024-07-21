#ifndef CGPRJ2023_24_SOUNDENGINE_HPP
#define CGPRJ2023_24_SOUNDENGINE_HPP
#include "../headers/soloud/include/soloud.h"
#include "../headers/soloud/include/soloud_wav.h"

class Sound
{
    friend class Project;
    SoLoud::Soloud gSoloud; // SoLoud engine
    SoLoud::Wav intro;
    SoLoud::Wav comicalExplosion;
    SoLoud::Wav zeppelinExploding;
    SoLoud::Wav victory;

public:
    void init() {
        gSoloud.init(); // Initialize SoLoud
        intro.load("../src/Sounds/Game-Start.wav");
        comicalExplosion.load("../src/Sounds/ComicalExplosion.wav");
        zeppelinExploding.load("../src/Sounds/ZeppExplosion1.wav");
        victory.load("../src/Sounds/Victory1.wav");
    }

    void playIntro()
    {
        gSoloud.play(intro); // Play the wave
    }

    void playComicalExplosion()
    {
        gSoloud.play(comicalExplosion);
    }

    void playZeppelinExpl()
    {
        gSoloud.play(zeppelinExploding);
    }

    void playVictory()
    {
        gSoloud.play(victory);
    }

    static double getSoundLength(SoLoud::Wav sound)
    {
        return sound.getLength();
    }

    ~Sound()
    {
        gSoloud.deinit();
    }
};

#endif //CGPRJ2023_24_SOUNDENGINE_HPP
