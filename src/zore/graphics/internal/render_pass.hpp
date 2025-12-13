#include "zore/graphics/graphics_core.hpp"
#include "zore/utils/sized_integer.hpp"
#include "zore/utils/memory.hpp"

namespace zore {

	class RenderPass {
    public:
        RenderPass(uint32_t attachment_count);
        RenderPass(const RenderPass&) = delete;
        RenderPass(RenderPass&&) = delete;
        RenderPass& operator=(const RenderPass&) = delete;
        RenderPass& operator=(RenderPass&&) = delete;
        ~RenderPass();

	public:
        static std::shared_ptr<RenderPass> Get(uint32_t attachment_count);
        const handle_t& Handle() const { return m_handle; }

    private:
        handle_t m_handle;
	};
}