#include <iostream>
#include "AudioWrapper.h"

int main()
{
    AudioSystem audio;
    
    if (!audio.init()) {
        std::cerr << "Init failed.\n";
        return -1;
    }

    audio.loadSound("sound", "audio/track1.mp3"); 
    audio.playSound("sound");

    std::cout << "Press ENTER to pause...\n";
    std::cin.get();
    audio.pauseSound("sound", true);

    std::cout << "Paused. Press ENTER to resume...\n";
    std::cin.get();
    audio.pauseSound("sound", false);

    std::cout << "Resumed. Press ENTER to stop...\n";
    std::cin.get();
    audio.stopSound("sound");

    std::cout << "Press ENTER to exit...\n";
    std::cin.get();

    audio.shutdown();
    return 0;
}
