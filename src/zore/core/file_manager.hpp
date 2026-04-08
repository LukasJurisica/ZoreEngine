#pragma once

#include "zore/utils/uuid.hpp"
#include <string>
#include <vector>
#include <fstream>

namespace zore {

	//========================================================================
	//	File Class
	//========================================================================

	class File {
	public:

		//------------------------------------------------------------------------
		//	File Management Utility
		//------------------------------------------------------------------------

		enum class CommonPaths { /*ASSETS, SHADERS, SCRIPTS, CONFIG, LOGS*/ LOCAL_APP_DATA };

		class Manager {
		public:
			static void Init(const std::string& path = "/");
			static std::string Path(const std::string& path);
			static std::string Path(CommonPaths path);
			static void EnsureDir(const std::string& path);
		};

		//------------------------------------------------------------------------
		//	File Class Iterator
		//------------------------------------------------------------------------

		class Iterator {
		public:
			explicit Iterator(std::fstream* stream);

			const std::string& operator*() const;
			Iterator& operator++();
			bool operator!=(const Iterator& other) const;

		private:
			std::fstream* m_stream;
			std::string m_line;
		};

		//------------------------------------------------------------------------
		//	File Class Utiltiy
		//------------------------------------------------------------------------

		enum class Mode { BINARY, TEXT };

	public:
		File(const std::string& filename, Mode mode = Mode::TEXT);
		static File Open(const std::string& filename, Mode mode = Mode::TEXT);
		File(const File&) = delete;
		File(File&& other) noexcept;
		File& operator=(const File&) = delete;
		File& operator=(File&& other) noexcept;
		~File();

		bool IsOpen() const;
		void Close();
		std::string Read();
		bool ReadLine(std::string& line);
		void Write(const std::string& content);
		void Append(const std::string& content);
		void Reset();
		size_t Size();

		Iterator begin();
		Iterator end();

	private:
		std::string m_filename;
		std::fstream m_stream;
		Mode m_mode;
	};
}