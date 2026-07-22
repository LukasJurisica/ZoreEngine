#include "zore/core/resource_manager.hpp"
#include "zore/structures/string_unordered_map.hpp"

static std::unordered_map<std::type_index, zore::string_unordered_map<void*>> s_resources;

namespace zore::Resource {

}