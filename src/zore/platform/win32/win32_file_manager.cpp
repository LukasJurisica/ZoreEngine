#include "zore/platform/win32/win32_core.hpp"
#include "zore/core/file_manager.hpp"
#include "zore/utils/string.hpp"
#include "zore/debug.hpp"
#include <filesystem>
#include <shlobj.h>

namespace zore {

	std::string File::Manager::Path(CommonPaths path) {
		switch (path) {
		case CommonPaths::LOCAL_APP_DATA: {
			PWSTR wstr_ptr = NULL;
			HRESULT hr = SHGetKnownFolderPath(FOLDERID_LocalAppData, 0, NULL, &wstr_ptr);
			if (SUCCEEDED(hr)) {
				std::filesystem::path local_app_data(wstr_ptr);
				CoTaskMemFree(wstr_ptr);
				return local_app_data.generic_string();
			}
		}
		};
		return "";
	}
}