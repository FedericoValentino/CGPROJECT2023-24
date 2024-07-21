#ifndef CGPRJ2023_24_SOUNDENGINE_HPP
#define CGPRJ2023_24_SOUNDENGINE_HPP
#include "../headers/soloud/include/soloud.h"
#include "../headers/soloud/include/soloud_wav.h"

class Sound
{
    SoLoud::Soloud gSoloud; // SoLoud engine
    SoLoud::Wav gWave;      // One wave file

public:
    void init()
    {
        gSoloud.init(); // Initialize SoLoud
        gWave.load("../src/Sounds/Game-Start.wav"); // Load a wave
    }

    void play()
    {
        gSoloud.play(gWave); // Play the wave
    }

    ~Sound()
    {
        gSoloud.deinit();
    }
};

#endif //CGPRJ2023_24_SOUNDENGINE_HPP
