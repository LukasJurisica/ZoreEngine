#include "zore/io/asset_pack.hpp"
#include "zore/debug.hpp"
#include <filesystem>
#include <cstring>
#include <fstream>

namespace zore {

	static uint32_t hash_key = 0x9E3779B9;

	//========================================================================
	//	Asset Pack
	//========================================================================

	AssetPack AssetPack::Load(std::string_view filename) {
		std::ifstream file(filename.data(), std::ios::binary | std::ios::ate);
		ENSURE(file.is_open(), "Failed to load asset pack: " + std::string(filename));

		std::streamsize size = file.tellg();
		file.seekg(0, std::ios::beg);
		ENSURE(size >= 4 && size % 4 == 0, "Asset pack file is corrupted");

		std::vector<char> buffer(size);
		file.read(buffer.data(), size);
		ENSURE(file, "Failed to read asset pack: " + std::string(filename));

		uint32_t* data = reinterpret_cast<uint32_t*>(buffer.data());
		for (size_t i = 0; i < buffer.size() / 4; i++)
			data[i] ^= hash_key;

		AssetPack pack;
		char* current = buffer.data();
		char* end = buffer.data() + buffer.size();

		uint32_t asset_count;
		std::memcpy(&asset_count, current, 4);
		current += 4;

		for (uint32_t i = 0; i < asset_count; ++i) {
			ENSURE(current + 4 <= end, "Asset pack file is corrupted");
			uint32_t key_size;
			std::memcpy(&key_size, current, 4);
			current += 4;

			ENSURE(current + key_size <= end, "Asset pack file is corrupted");
			std::string key(current, key_size);
			current += key_size;

			ENSURE(current + 4 <= end, "Asset pack file is corrupted");
			uint32_t value_size;
			std::memcpy(&value_size, current, 4);
			current += 4;

			ENSURE(current + value_size <= end, "Asset pack file is corrupted");
			pack.m_assets[key] = std::vector<char>(current, current + value_size);
			current += value_size;
		}

		ENSURE(current <= end, "Asset pack file is corrupted");
		return pack;
	}

	AssetPack AssetPack::Create(std::vector<std::string_view> assets) {
		AssetPack pack;
		for (std::string_view asset : assets)
			pack.Add(asset);
		return pack;
	}

	AssetPack& AssetPack::Add(std::string_view path, std::string_view root) {
		if (path.empty() || !std::filesystem::exists(path))
			return *this;
		std::string key = (std::filesystem::path(root) / std::filesystem::path(path).filename()).generic_string();
		if (std::filesystem::is_directory(path))
			AddFolder(path, key);
		else
			AddFile(path, key);
		return *this;
	}

	void AssetPack::Save(std::string_view filename) {
		uint32_t asset_count = static_cast<uint32_t>(m_assets.size());
		uint32_t buffer_size = 4;
		for (const auto& [key, value] : m_assets)
			buffer_size += static_cast<uint32_t>(key.size()) + static_cast<uint32_t>(value.size()) + 8;
		buffer_size = (buffer_size / 4) * 4 + 4;
		std::vector<char> buffer(buffer_size);

		char* current = buffer.data();
		std::memcpy(current, &asset_count, 4);
		current += 4;
		for (const auto& [key, value] : m_assets) {
			uint32_t key_size = static_cast<uint32_t>(key.size());
			std::memcpy(current, &key_size, 4);
			current += 4;
			std::memcpy(current, key.data(), key_size);
			current += key_size;
			uint32_t value_size = static_cast<uint32_t>(value.size());
			std::memcpy(current, &value_size, 4);
			current += 4;
			std::memcpy(current, value.data(), value_size);
			current += value_size;
		}

		uint32_t* data = reinterpret_cast<uint32_t*>(buffer.data());
		for (size_t i = 0; i < buffer.size() / 4; i++)
			data[i] ^= hash_key;
		
		std::ofstream file(filename.data(), std::ios::binary);
		ENSURE(file.is_open(), "Failed to save asset pack: " + std::string(filename));
		file.write(buffer.data(), buffer.size());
	}

	bool AssetPack::Has(std::string_view path) {
		return m_assets.find(path) != m_assets.end();
	}

	std::span<const char> AssetPack::Get(std::string_view path) {
		auto iter = m_assets.find(path);
		if (iter != m_assets.end())
			return std::span<const char>(iter->second.data(), iter->second.size());
		return std::span<const char>();
	}

	void AssetPack::AddFolder(std::string_view path, std::string_view root) {
		for (const auto& entry : std::filesystem::recursive_directory_iterator(path)) {
			if (!entry.is_regular_file())
				continue;
			std::filesystem::path relative = std::filesystem::relative(entry.path(), path);
			std::string key = (std::filesystem::path(root) / relative).generic_string();
			AddFile(entry.path().string(), key);
		}
	}

	void AssetPack::AddFile(std::string_view path, std::string_view key) {
		std::ifstream file(path.data(), std::ios::binary | std::ios::ate);
		ENSURE(file.is_open(), "Failed to add file to asset pack: " + std::string(path));

		std::streamsize size = file.tellg();
		file.seekg(0, std::ios::beg);
		std::vector<char> buffer(size);
		file.read(buffer.data(), size);

		m_assets[std::string(key)] = std::move(buffer);
	}
}