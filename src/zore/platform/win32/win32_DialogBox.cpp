#include "zore/utils/DialogBox.hpp"
#define WIN32_NOT_LEAN
#include "zore/platform/win32/win32_Core.hpp"
#include "zore/devices/Window.hpp"
#include "zore/Debug.hpp"

#define CHAR_BUFFER_SIZE 256

namespace zore {

	void CreateFilterStringFromVector(char* filter, const std::vector<FilterDesc>& filters, bool includeAllFiles) {
		int index = 0;

		if (includeAllFiles) {
			const char allFileDesc[] = "All Files\0*.*";
			memcpy(filter, allFileDesc, sizeof(allFileDesc));
			index = sizeof(allFileDesc);
		}

		for (const FilterDesc& fd : filters) {
			int fddl = static_cast<int>(fd.desc.length());
			int fdel = static_cast<int>(fd.ext.length());
			if (index + fddl + fdel + 5 > CHAR_BUFFER_SIZE) {
				Logger::Warn("Could not create File Open/Save Dialog filter. The size of the char buffer was too small to accommodate the filter");
				return;
			}

			fd.desc.copy(filter + index, fddl);
			index += fddl;
			filter[index++] = '\0';

			filter[index++] = '*';
			filter[index++] = '.';
			fd.ext.copy(filter + index, fdel);
			index += fdel;
			filter[index++] = '\0';
		}
		filter[index] = '\0';
	}

	void InitializeOFN(OPENFILENAME& ofn, char* szFile, char* filter, const std::vector<FilterDesc>& filters, bool includeAllFiles, DWORD flags) {
		CreateFilterStringFromVector(filter, filters, includeAllFiles);
		ZeroMemory(&ofn, sizeof(OPENFILENAME));
		ofn.lStructSize = sizeof(OPENFILENAME);
		Window::GetNativeHandle(&ofn.hwndOwner);
		ofn.lpstrFile = szFile;
		ofn.nMaxFile = CHAR_BUFFER_SIZE;
		ofn.lpstrFilter = filter;
		ofn.nFilterIndex = 1 + (includeAllFiles ? 1 : 0);
		//ofn.lpstrFileTitle = NULL;
		//ofn.nMaxFileTitle = 0;
		//ofn.lpstrInitialDir = NULL;
		ofn.Flags = flags;
	}

	std::string Dialog::OpenFile(const std::vector<FilterDesc>& filters, bool includeAllFiles) {
		OPENFILENAME ofn;
		char szFile[CHAR_BUFFER_SIZE];
		char filter[CHAR_BUFFER_SIZE];
		InitializeOFN(ofn, szFile, filter, filters, includeAllFiles, OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR);

		if (GetOpenFileName(&ofn))
			return ofn.lpstrFile;

		return "";
	}

	std::string Dialog::SaveFile(const std::vector<FilterDesc>& filters, bool includeAllFiles) {
		OPENFILENAME ofn;
		char szFile[CHAR_BUFFER_SIZE];
		char filter[CHAR_BUFFER_SIZE];
		InitializeOFN(ofn, szFile, filter, filters, includeAllFiles, OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR);

		if (GetSaveFileName(&ofn)) {
			std::string result = ofn.lpstrFile;
			int filterIndex = ofn.nFilterIndex - (includeAllFiles ? 1 : 0) - 1;
			if (filterIndex >= 0 && result.rfind(".") == std::string::npos)
				result += "." + filters[filterIndex].ext;
			return result;
		}

		return "";
	}
}