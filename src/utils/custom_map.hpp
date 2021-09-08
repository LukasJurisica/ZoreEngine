//#pragma once
//
//#include <vector>
//
//namespace zore {
//
//	class custom_map {
//	public:
//		custom_map() = default;
//		~custom_map() = default;
//
//		custom_map(custom_map&&) = delete; // Move constructor
//		custom_map(const custom_map&) = delete; // Copy Constructor
//		custom_map& operator=(custom_map&&) = delete; // Move assignment
//		custom_map& operator=(const custom_map&) = delete; // Copy assignment
//
//		//std::vector<std::pair<K, V>>::iterator begin() { return data.begin(); }
//		//std::vector<std::pair<K, V>>::iterator end() { return data.end(); }
//		//std::vector<std::pair<K, V>>::const_iterator begin() const { return data.begin(); }
//		//std::vector<std::pair<K, V>>::const_iterator end() const { return data.end(); }
//
//		unsigned int& operator[](const std::string& key) {
//			for (std::pair<std::string, unsigned int>& d : data)
//				if (key == d.first)
//					return d.second;
//			return 0;
//		}
//
//		std::vector<std::pair<std::string, unsigned int>> data;
//	};
//}