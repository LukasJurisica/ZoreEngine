#include "zore/audio/group.hpp"
#include "zore/audio/manager.hpp"
#include <unordered_map>
#include <miniaudio.h>

namespace zore::audio {

    static std::unordered_map<std::string, ma_sound_group> s_groups;

    Group::Group(const std::string& name) : m_name(name) {
        auto result = s_groups.insert({ name, ma_sound_group() });
        if (result.second)
            ma_sound_group_init(Manager::GetEngine(), 0, nullptr, &result.first->second);
    }

    Group::Group(const std::string& name, const Group& parent) : Group(name, parent.m_name) {}
    
    Group::Group(const std::string& name, const std::string& parent_name) {
        auto result = s_groups.insert({ name, ma_sound_group() });
        if (result.second) {
            auto iter = s_groups.find(parent_name);
            ma_sound_group* parent = iter != s_groups.end() ? &iter->second : nullptr;
            ma_sound_group_init(Manager::GetEngine(), 0, parent, &result.first->second);
        }
    }

    Group::~Group() {
        
    }

    void Group::SetVolume(float volume) {
        auto iter = s_groups.find(m_name);
        if (iter != s_groups.end())
            ma_sound_group_set_volume(&iter->second, volume);
    }

    void Group::SetEffect(Effect* effect) {
        auto iter = s_groups.find(m_name);
        if (iter != s_groups.end())
            ma_node_attach_output_bus(&iter->second, 0, effect ? reinterpret_cast<ma_node*>(effect) : nullptr, 0);
    }
}