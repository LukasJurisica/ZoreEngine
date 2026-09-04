#include "zore/core/resource_manager.hpp"

namespace zore::Resource {

	void Manager::Cleanup() {
		for (auto& [type, resources] : s_resources)
			for (auto& [name, resource] : resources)
				delete resource;
		s_resources.clear();
	}
}