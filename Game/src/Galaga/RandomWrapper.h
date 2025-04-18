#pragma once

#include <map>
#include <random>
#include <string>

namespace Galaga
{
    class RandomWrapper final
    {
    public:
        RandomWrapper() : _rand_generator(std::default_random_engine(_rand_device()))
        {

        }

        void add_uniform_real_distribution(const std::string& name, float lower_bound, float upper_bound)
        {
            _uniform_real_distro_map.insert_or_assign(name, std::uniform_real_distribution{ lower_bound, upper_bound });
        }

        float get_uniform_real()
        {
            return _default_uniform_real_distro(_rand_generator);
        }

        float get_uniform_real(const std::string& name)
        {
            if (_uniform_real_distro_map.contains(name))
            {
                return _uniform_real_distro_map[name](_rand_generator);
            }
            return get_uniform_real();
        }

    private:
        std::random_device _rand_device;
        std::default_random_engine _rand_generator;
        std::uniform_real_distribution<float> _default_uniform_real_distro;
        std::map<std::string, std::uniform_real_distribution<float>> _uniform_real_distro_map;

    };
}