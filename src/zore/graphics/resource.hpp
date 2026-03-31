#pragma once

#include "zore/utils/sized_integer.hpp"
#include <atomic>

namespace zore {

	//========================================================================
	//	Render Engine Interface
	//========================================================================

	class Resource {
	public:
		Resource();
		~Resource() = default;

	public:
		uint32_t GetID() const;
		void Acquire();
		void Release();

	protected:
		virtual void Delete() = 0;
		bool IsValid() const;

	private:
		uint32_t m_ref_count;
		uint32_t m_id;
	};
}