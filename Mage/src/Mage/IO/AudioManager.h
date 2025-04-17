#pragma once
#include "../Core/Core.h"
namespace Mage
{
    class MAGE_API AudioManager
    {
        friend class Application;

    public:
        AudioManager(AudioManager &&) = delete;

        AudioManager(const AudioManager &) = delete;

        AudioManager &operator=(AudioManager &&) = delete;

        AudioManager &operator=(const AudioManager &) = delete;

        ~AudioManager();

         void set_mixer_group_volume(const char *mixer_group, float volume);

        bool load_sound(const char *key, const char *mixer_group, const char *sound_file);

        bool play_sound(const char *key, bool loop = false);

        bool stop_sound(const char *key);

    private:
        AudioManager();

        struct Impl;
        Impl *_impl;
    };
}
