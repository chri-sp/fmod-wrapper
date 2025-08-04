#include <iostream>
#include <vector>
#include "AudioWrapper.h"

void printMenu();
std::string getKeyFromFilename(const std::string& filename);
std::string chooseSoundKey(const std::vector<std::string>& keys);

int main()
{
    AudioSystem audio;
    std::vector<std::string> loadedSounds;

    if (!audio.init())
    {
        std::cerr << "Init failed.\n";
        return -1;
    }

    int choice = 0;
    while (choice != 9)
    {
        audio.update();

        printMenu();

        std::cin >> choice;
        switch (choice)
        {
        // Load sound
        case 1:
        case 2:
            {
                std::cout << "Enter sound filename (with extension) in audio folder: ";
                std::string soundName;
                std::cin >> soundName;

                char loopInput;
                std::cout << "Enable loop? (y/n): ";
                std::cin >> loopInput;
                bool loop = (loopInput == 'y' || loopInput == 'Y');

                std::string key = getKeyFromFilename(soundName);
                std::string path = "audio/" + soundName;
                bool streaming = (choice == 2);

                if (audio.loadSound(key, path, loop, streaming))
                {
                    std::cout << (streaming ? "Streaming" : "Static") << " sound loaded from " << path << ".\n";

                    // Add key only if not present
                    if (std::find(loadedSounds.begin(), loadedSounds.end(), key) == loadedSounds.end())
                        loadedSounds.push_back(key);
                }
                else
                    std::cout << "Failed to load sound from " << path << ".\n";
                break;
            }

        // Play
        case 3:
            {
                if (loadedSounds.empty())
                {
                    std::cout << "No sounds loaded yet.\n";
                    break;
                }
                std::string key = chooseSoundKey(loadedSounds);
                if (!key.empty())
                {
                    audio.playSound(key);
                }
                break;
            }

        // Pause
        case 4:
            {
                if (loadedSounds.empty())
                {
                    std::cout << "No sounds loaded yet.\n";
                    break;
                }
                std::string key = chooseSoundKey(loadedSounds);
                if (!key.empty())
                {
                    audio.pauseSound(key, true);
                    std::cout << "Paused.\n";
                }
                break;
            }

        // Resume
        case 5:
            {
                if (loadedSounds.empty())
                {
                    std::cout << "No sounds loaded yet.\n";
                    break;
                }
                std::string key = chooseSoundKey(loadedSounds);
                if (!key.empty())
                {
                    audio.pauseSound(key, false);
                    std::cout << "Resumed.\n";
                }
                break;
            }

        // Stop
        case 6:
            {
                if (loadedSounds.empty())
                {
                    std::cout << "No sounds loaded yet.\n";
                    break;
                }
                std::string key = chooseSoundKey(loadedSounds);
                if (!key.empty())
                {
                    audio.stopSound(key);
                    std::cout << "Stopped.\n";
                }
                break;
            }

        // Set volume
        case 7:
            {
                if (loadedSounds.empty())
                {
                    std::cout << "No sounds loaded yet.\n";
                    break;
                }
                std::string key = chooseSoundKey(loadedSounds);
                if (!key.empty())
                {
                    float vol;
                    std::cout << "Enter volume (0.0 to 1.0): ";
                    std::cin >> vol;
                    audio.setVolume(key, vol);
                    std::cout << "Volume changed.\n";
                }
                break;
            }

        // Set pan
        case 8:
            {
                if (loadedSounds.empty())
                {
                    std::cout << "No sounds loaded yet.\n";
                    break;
                }
                std::string key = chooseSoundKey(loadedSounds);
                if (!key.empty())
                {
                    float pan;
                    std::cout << "Enter pan (-1.0 left to 1.0 right): ";
                    std::cin >> pan;
                    audio.setPan(key, pan);
                    std::cout << "Pan changed.\n";
                }
                break;
            }

        case 9:
            break;

        default:
            std::cout << "Invalid option.\n";
        }
    }

    audio.shutdown();
    return 0;
}


void printMenu()
{
    std::cout << "\n=== FMOD Wrapper Demo ===\n";
    std::cout << "1) Load static sound\n";
    std::cout << "2) Load streaming sound\n";
    std::cout << "3) Play sound\n";
    std::cout << "4) Pause sound\n";
    std::cout << "5) Resume sound\n";
    std::cout << "6) Stop sound\n";
    std::cout << "7) Set volume\n";
    std::cout << "8) Set pan\n";
    std::cout << "9) Exit\n";
    std::cout << "\nSelect option: ";
}


// Helper function to extract key from filename (without extension)
std::string getKeyFromFilename(const std::string& filename)
{
    size_t pos = filename.find_last_of('.');
    if (pos == std::string::npos)
        return filename;
    return filename.substr(0, pos);
}

// Lets user select a loaded sound key
std::string chooseSoundKey(const std::vector<std::string>& keys)
{
    std::cout << "Select a loaded sound:\n";
    for (size_t i = 0; i < keys.size(); ++i)
        std::cout << i + 1 << ") " << keys[i] << "\n";
    std::cout << "Choice (number): ";

    int choice;
    std::cin >> choice;

    if (choice < 1 || static_cast<size_t>(choice) > keys.size())
    {
        std::cout << "Invalid choice.\n";
        return "";
    }

    return keys[choice - 1];
}
