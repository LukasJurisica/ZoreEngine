#pragma once

#include "zore/structures/string_unordered_map.hpp"
#include <vector>
#include <string>
#include <span>

namespace zore {

	//========================================================================
	//	Asset Pack
	//========================================================================

	class AssetPack {
	public:
		AssetPack() = default;
		~AssetPack() = default;

		static AssetPack Load(std::string_view filename);
		static AssetPack Create(std::vector<std::string_view> assets = {});

		AssetPack& Add(std::string_view path, std::string_view root = "");
		void Save(std::string_view filename);
		bool Has(std::string_view path);
		std::span<const char> Get(std::string_view path);

	private:

		void AddFolder(std::string_view path, std::string_view root);
		void AddFile(std::string_view path, std::string_view key);

	private:
		zore::string_unordered_map<std::vector<char>> m_assets;
	};
}