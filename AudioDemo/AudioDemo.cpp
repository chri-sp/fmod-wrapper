#include <iostream>
#include "AudioWrapper.h"

int main()
{
    AudioSystem audio;
    
    if (!audio.init()) {
        std::cerr << "Init failed.\n";
        return -1;
    }
/*
//  TEST load, play, pause, stop
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
*/

    // TEST loop and one-shot
    audio.loadSound("loop", "audio/track2.mp3", true);
    audio.playSound("loop");

    std::cout << "Press ENTER to stop...\n";
    std::cin.get();
    audio.stopSound("loop");

    std::cout << "Press ENTER to play one-shot...\n";
    std::cin.get();

    audio.loadSound("one-shot", "audio/track2.mp3", false);
    audio.playSound("one-shot");

    std::cout << "Press ENTER to exit...\n";
    std::cin.get();

    audio.shutdown();
    return 0;
}
