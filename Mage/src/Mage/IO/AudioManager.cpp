//
// Created by alecp on 4/15/2025.
//
#include "AudioManager.h"
#include "miniaudio.h"

namespace Mage
{
    struct AudioManager::Impl
    {
        ma_engine engine;
        std::unordered_map<std::string, ma_sound*> sounds;
        std::unordered_map<std::string, std::string> sounds_to_mixer_groups;
        std::unordered_map<std::string, float> mixer_group_volumes;

        void cleanup_sound_resources(ma_sound* sound, const std::string& key)
        {
            if (sound)
            {
                LOG_E_INFO("Cleaning up sound with key %s", key.c_str());
                ma_sound_stop(sound);
                ma_sound_uninit(sound);
                delete sound;
            }
        }
    };

    AudioManager::AudioManager()
    {
        LOG_E_INFO("AudioManager starting");
        _impl = new Impl();

        auto engine_config = ma_engine_config_init();
        auto result = ma_engine_init(&engine_config, &_impl->engine);
        if (result != MA_SUCCESS)
        {
            throw Exception((std::string("Failed initializing miniaudio: ")
                + ma_result_description(result)).c_str());
        }
    }

    AudioManager::~AudioManager()
    {
        LOG_E_INFO("AudioManager stopping");
        for (const auto& s : _impl->sounds)
        {
            _impl->cleanup_sound_resources(s.second, s.first);
        }
        ma_engine_uninit(&_impl->engine);
        delete _impl;
    }

    void AudioManager::set_mixer_group_volume(const char *mixer_group, float volume)
    {
        volume = std::min(1.0f, std::max(0.0f, volume));
        _impl->mixer_group_volumes[mixer_group] = volume;
    }

    bool AudioManager::load_sound(const char *key, const char *mixer_group, const char *sound_file)
    {
        auto str_key = std::string(key);
        auto str_mixer_group = std::string(mixer_group);
        if (!_impl->mixer_group_volumes.contains(str_mixer_group))
        {
            _impl->mixer_group_volumes[str_mixer_group] = 1.0f;
        }
        if (_impl->sounds.contains(str_key))
        {
            LOG_E_WARN("Loading sound file %s with key %s, but that key is already in use. "
                       "Unloading previous sound", sound_file, key);
            _impl->cleanup_sound_resources(_impl->sounds[str_key], str_key);
            _impl->sounds.erase(str_key);
        }

        _impl->sounds_to_mixer_groups[str_key] = str_mixer_group;
        auto sound = new ma_sound();
        auto result = ma_sound_init_from_file(&_impl->engine, sound_file,
            MA_SOUND_FLAG_DECODE | MA_SOUND_FLAG_NO_PITCH | MA_SOUND_FLAG_NO_SPATIALIZATION,
            nullptr, nullptr, sound);
        if (result != MA_SUCCESS)
        {
            LOG_E_ERROR("Failed to load sound file %s with key %s: %s", sound_file, key, ma_result_description(result));
            delete sound;
            return false;
        }

        _impl->sounds[str_key] = sound;
        LOG_E_INFO("Successfully loaded sound file: %s with key %s.", sound_file, key);
        return true;
    }


    bool AudioManager::play_sound(const char *key, bool loop)
    {
        auto str_key = std::string(key);
        auto it = _impl->sounds.find(str_key);
        if (it == _impl->sounds.end())
        {
            LOG_E_WARN("Failed to play sound with key %s because the key was not found.", key);
            return false;
        }
        auto sound = it->second;
        auto mixer_group = _impl->sounds_to_mixer_groups[str_key];
        auto volume = _impl->mixer_group_volumes[mixer_group];
        ma_sound_set_volume(sound, volume);
        ma_sound_seek_to_pcm_frame(sound, 0);
        ma_sound_set_looping(sound, loop ? MA_TRUE : MA_FALSE);
        auto result = ma_sound_start(sound);
        if (result != MA_SUCCESS)
        {
            LOG_E_WARN("Failed to play sound with key %s: %s", key, ma_result_description(result));
            return false;
        }
        return true;
    }

    bool AudioManager::stop_sound(const char *key)
    {
        return false;
    }
}
