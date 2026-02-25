#define WIN32_NOT_LEAN
#include "zore/core/dialog_box.hpp"
#include "zore/platform/win32/win32_core.hpp"
#include "zore/devices/window.hpp"
#include "zore/debug.hpp"

#define CHAR_BUFFER_SIZE 256

namespace zore {

	void CreateFilterStringFromVector(char* filter, const std::vector<FilterDesc>& filters, bool include_all_files) {
		int index = 0;

		if (include_all_files) {
			const char all_file_description[] = "All Files\0*.*";
			memcpy(filter, all_file_description, sizeof(all_file_description));
			index = sizeof(all_file_description);
		}

		for (const FilterDesc& fd : filters) {
			int fddl = static_cast<int>(fd.description.length());
			int fdel = static_cast<int>(fd.extension.length());
			if (index + fddl + fdel + 5 > CHAR_BUFFER_SIZE) {
				Logger::Warn("Could not create File Open/Save Dialog filter. The size of the char buffer was too small to accommodate the filter");
				return;
			}

			fd.description.copy(filter + index, fddl);
			index += fddl;
			filter[index++] = '\0';

			filter[index++] = '*';
			filter[index++] = '.';
			fd.extension.copy(filter + index, fdel);
			index += fdel;
			filter[index++] = '\0';
		}
		filter[index] = '\0';
	}

	void InitializeOFN(OPENFILENAME& ofn, char* szFile, char* filter, const std::vector<FilterDesc>& filters, bool include_all_files, DWORD flags) {
		CreateFilterStringFromVector(filter, filters, include_all_files);
		ZeroMemory(&ofn, sizeof(OPENFILENAME));
		ZeroMemory(szFile, CHAR_BUFFER_SIZE);
		ofn.lStructSize = sizeof(OPENFILENAME);
		Window::GetNativeHandle(&ofn.hwndOwner);
		ofn.lpstrFile = szFile;
		ofn.nMaxFile = CHAR_BUFFER_SIZE;
		ofn.lpstrFilter = filter;
		ofn.nFilterIndex = 1 + (include_all_files ? 1 : 0);
		//ofn.lpstrFileTitle = NULL;
		//ofn.nMaxFileTitle = 0;
		//ofn.lpstrInitialDir = NULL;
		ofn.Flags = flags;
	}

	std::string Dialog::OpenFile(const std::vector<FilterDesc>& filters, bool include_all_files) {
		OPENFILENAME ofn;
		char szFile[CHAR_BUFFER_SIZE];
		char filter[CHAR_BUFFER_SIZE];
		InitializeOFN(ofn, szFile, filter, filters, include_all_files, OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR);

		if (GetOpenFileName(&ofn))
			return ofn.lpstrFile;

		return "";
	}

	std::string Dialog::SaveFile(const std::vector<FilterDesc>& filters, bool include_all_files) {
		OPENFILENAME ofn;
		char szFile[CHAR_BUFFER_SIZE];
		char filter[CHAR_BUFFER_SIZE];
		InitializeOFN(ofn, szFile, filter, filters, include_all_files, OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR);

		if (GetSaveFileName(&ofn)) {
			std::string result = ofn.lpstrFile;
			int filterIndex = ofn.nFilterIndex - (include_all_files ? 1 : 0) - 1;
			if (filterIndex >= 0 && result.rfind(".") == std::string::npos)
				result += "." + filters[filterIndex].extension;
			return result;
		}

		return "";
	}
}