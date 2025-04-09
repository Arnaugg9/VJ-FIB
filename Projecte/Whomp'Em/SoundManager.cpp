#include "SoundManager.h"

ISoundEngine* SoundManager::engine = createIrrKlangDevice();
ISound* SoundManager::currentMusic = nullptr;

void SoundManager::playMusic(const char* path, bool loop) {
    if (!engine) return;

    if (currentMusic) {
        currentMusic->stop();
        currentMusic->drop();
    }

    currentMusic = engine->play2D(path, loop, false, true);
	if (currentMusic) currentMusic->setVolume(0.5f);
}

void SoundManager::stopMusic() {
    if (currentMusic) {
        currentMusic->stop();
        currentMusic->drop();
        currentMusic = nullptr;
    }
}

void SoundManager::cleanup() {
    stopMusic();
    if (engine) {
        engine->drop();
        engine = nullptr;
    }
}

void SoundManager::playSFX(const char* path) {
    if (!engine) return;
    engine->play2D(path, false);
}
