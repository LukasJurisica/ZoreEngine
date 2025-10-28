#pragma once

#include <string>

namespace zore::audio {

    class Effect;

    class Group {
    public:
        Group(const std::string& name);
        Group(const std::string& name, const Group& parent);
        Group(const std::string& name, const std::string& parent_name);
        ~Group();

        void SetVolume(float volume);
        void SetEffect(Effect* effect);
        
    private:
        const std::string m_name;
    };
}