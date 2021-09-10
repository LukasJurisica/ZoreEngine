#pragma once
#include "utils/DataTypes.hpp"
#include <string>

namespace zore {

	class Texture2D {
	public:
		static Texture2D* Create(const std::string& name);
		static Texture2D* Create(uint width, uint height, uint channels = 4);
		virtual ~Texture2D() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;
		virtual void SetSize(uint width, uint height) = 0;
	};
}