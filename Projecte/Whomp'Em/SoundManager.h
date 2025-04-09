#ifndef SOUNDMANAGER_H
#define SOUNDMANAGER_H

#include <irrKlang.h>
using namespace irrklang;

class SoundManager {
public:
    static ISoundEngine* engine;
    static ISound* currentMusic;

    static void playMusic(const char* path, bool loop = true);
    static void stopMusic();
    static void cleanup();
	static void playSFX(const char* path);
};

#endif // SOUNDMANAGER_H