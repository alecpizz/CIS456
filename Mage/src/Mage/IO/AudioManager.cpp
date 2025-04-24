//
// Created by alecp on 4/15/2025.
//
#include "AudioManager.h"
#include "miniaudio.h"

namespace Mage
{
    struct CallbackInfo
    {
        void (*function)(void*user_data) = nullptr;
        void* user_data = nullptr;
    };

    struct AudioManager::Impl
    {
        ma_engine engine;
        std::unordered_map<std::string, ma_sound*> sounds;
        std::unordered_map<std::string, std::string> sounds_to_mixer_groups;
        std::unordered_map<std::string, float> mixer_group_volumes;
        std::unordered_map<ma_sound*, CallbackInfo> sound_end_callbacks;

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

        // ReSharper disable once CppParameterMayBeConstPtrOrRef
        static void miniaudio_end_callback(void* user_data, ma_sound* sound)
        {
            auto am = static_cast<AudioManager*>(user_data);
            if (!am || !sound)
            {
                return;
            }
            auto it = am->_impl->sound_end_callbacks.find(sound);
            if (it != am->_impl->sound_end_callbacks.end())
            {
                //copy the callback info before resetting it
                CallbackInfo ci = it->second;
                //reset the callback info in the map before calling the function to make it one shot
                //and prevent potential recursion issues
                it->second = {};
                if (ci.function != nullptr)
                {
                    try
                    {
                        ci.function(ci.user_data);
                    }
                    catch (...)
                    {
                        std::string msg = "Exception thrown from on_end_callback function supplied "
                                          "in call to AudioManager::play_sound";
                        auto matching_sound_found = false;
                        for (auto& s : am->_impl->sounds)
                        {
                            if (sound == s.second)
                            {
                                msg += " (sound key: " + s.first + ")";
                                matching_sound_found = true;
                                break;
                            }
                        }

                        if (!matching_sound_found)
                        {
                            msg += " (no sound key match found)";
                        }
                        LOG_E_ERROR(msg.c_str());
                    }
                }
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

        //Add default-initialized CallbackInfo entry for this sound
        _impl->sound_end_callbacks[sound] = {};
        ma_sound_set_end_callback(sound, Impl::miniaudio_end_callback, this);

        LOG_E_INFO("Successfully loaded sound file: %s with key %s.", sound_file, key);
        return true;
    }


    bool AudioManager::play_sound(const char *key, bool loop,
            void(*on_end_callback)(void* user_data), void* callback_data)
    {
        auto str_key = std::string(key);
        auto it = _impl->sounds.find(str_key);
        if (it == _impl->sounds.end())
        {
            LOG_E_WARN("Failed to play sound with key %s because the key was not found.", key);
            return false;
        }
        auto sound = it->second;

        if (loop)
        {
            if (on_end_callback != nullptr)
            {
                LOG_E_WARN("Sound with key %s called with loop=true and non null on_end_callback", key);
            }
            _impl->sound_end_callbacks[sound]= {};
        }
        else
        {
            _impl->sound_end_callbacks[sound] = {on_end_callback, callback_data};
        }

        auto mixer_group = _impl->sounds_to_mixer_groups[str_key];
        auto volume = _impl->mixer_group_volumes[mixer_group];
        ma_sound_set_volume(sound, volume);
        ma_sound_seek_to_pcm_frame(sound, 0);
        ma_sound_set_looping(sound, loop ? MA_TRUE : MA_FALSE);
        auto result = ma_sound_start(sound);
        if (result != MA_SUCCESS)
        {
            LOG_E_WARN("Failed to play sound with key %s: %s", key, ma_result_description(result));
            _impl->sound_end_callbacks[sound]= {};
            return false;
        }
        return true;
    }

    bool AudioManager::stop_sound(const char *key)
    {
        auto str_key = std::string(key);
        auto it = _impl->sounds.find(str_key);
        if (it == _impl->sounds.end())
        {
            LOG_E_WARN("Failed to stop sound with key %s because that key was not found.", key);
            return false;
        }
        auto sound = it->second;
        //clear any pending CallBackInfo when manually stopping
        _impl->sound_end_callbacks[sound] = {};
        ma_sound_stop(sound);

        //Not checking the return of this call because the most likely reason for failure is that
        //the sound wasn't playing at the time the call was made
        return true;
    }
}
