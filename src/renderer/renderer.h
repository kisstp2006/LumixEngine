#pragma once

#include "engine/lumix.h"

#include "core/allocator.h"
#include "core/color.h"
#include "core/math.h"
#include "core/profiler.h"
#include "draw_stream.h"
#include "engine/plugin.h"
#include "gpu/gpu.h"

#ifndef _WIN32
	#include "engine/engine.h"
#endif

namespace Lumix {

enum class AttributeSemantic : u8;
struct GBuffer;
enum class MaterialIndex : u32;

struct RenderbufferDesc {
	IVec2 size;
	gpu::TextureFormat format;
	gpu::TextureFlags flags = gpu::TextureFlags::RENDER_TARGET | gpu::TextureFlags::NO_MIPS;
	const char* debug_name;
};

enum class RenderBufferHandle : u32 {};

static constexpr RenderBufferHandle INVALID_RENDERBUFFER = RenderBufferHandle{0xffFFffFF};

struct LUMIX_RENDERER_API RenderPlugin {
	virtual ~RenderPlugin() {}
	// all `RenderPlugin` functions are called during execution of every `Pipeline`
	virtual void renderUI(struct Pipeline& pipeline) {}
	virtual void renderOpaque(Pipeline& pipeline) {}
	virtual void renderTransparent(Pipeline& pipeline) {}
	virtual void renderBeforeLightPass(const GBuffer& gbuffer, Pipeline& pipeline) {}
	virtual RenderBufferHandle renderBeforeTonemap(const GBuffer& gbuffer, RenderBufferHandle input, Pipeline& pipeline);
	virtual RenderBufferHandle renderBeforeTransparent(const GBuffer& gbuffer, RenderBufferHandle input, Pipeline& pipeline);
	virtual RenderBufferHandle renderAfterTonemap(const GBuffer& gbuffer, RenderBufferHandle input, Pipeline& pipeline);
	// returns valid buffer if plugin's Antialiasing run and no other AA should run
	virtual RenderBufferHandle renderAA(const GBuffer& gbuffer, RenderBufferHandle input, Pipeline& pipeline);
	// returns true if plugin's tonemap run and builtin tonemap should not run
	virtual bool tonemap(RenderBufferHandle input, RenderBufferHandle& output, Pipeline& pipeline);
	
	virtual void debugUI(Pipeline&) {}
	virtual bool debugOutput(RenderBufferHandle input, Pipeline& pipeline);
	
	virtual void pipelineDestroyed(Pipeline& pipeline) {}
	virtual void frame(struct Renderer& renderer) {}
};

struct DrawStream;
struct UniformPool;
struct TransientPool;

struct LUMIX_RENDERER_API Renderer : ISystem {
	struct MemRef {
		u32 size = 0;
		void* data = nullptr;
		bool own = false;
	};

	enum { MAX_SHADER_DEFINES = 32 };

	virtual void frame() = 0;
	virtual u32 frameNumber() const = 0;
	virtual void waitForRender() = 0;
	virtual void waitForCommandSetup() = 0;
	virtual void waitCanSetup() = 0;
	virtual struct Engine& getEngine() = 0;
	virtual float getLODMultiplier() const = 0;
	virtual void setLODMultiplier(float value) = 0;
	
	virtual struct ArenaAllocator& getCurrentFrameAllocator() = 0;
	virtual IAllocator& getAllocator() = 0;
	virtual MemRef allocate(u32 size) = 0;
	virtual MemRef copy(const void* data, u32 size) = 0 ;
	virtual void free(const MemRef& memory) = 0;

	virtual void addPlugin(RenderPlugin& plugin) = 0;
	virtual void removePlugin(RenderPlugin& plugin) = 0;
	virtual Span<RenderPlugin*> getPlugins() = 0;

	virtual u8 getShaderDefineIdx(const char* define) = 0;
	virtual const char* getShaderDefine(int define_idx) const = 0;
	virtual int getShaderDefinesCount() const = 0;
	virtual u8 getLayerIdx(const char* name) = 0;
	virtual u8 getLayersCount() const = 0;
	virtual const char* getLayerName(u8 layer) const = 0;
	virtual u32 allocSortKey(u32 hash) = 0;
	virtual void freeSortKey(u32 key) = 0;
	virtual u32 getMaxSortKey() const = 0;
	virtual void enableBuiltinTAA(bool enable) = 0;
	
	virtual const char* getSemanticDefines(Span<const AttributeSemantic> attributes) = 0;

	virtual struct FontManager& getFontManager() = 0;
	virtual struct ResourceManager& getTextureManager() = 0;
	
	virtual MaterialIndex createMaterialConstants(Span<const float> data) = 0;
	virtual MaterialIndex createMaterialInstance(Span<const float> data) = 0;
	virtual void updateMaterialConstants(MaterialIndex handle, Span<const float> data, u32 offset) = 0;
	virtual void destroyMaterialConstants(MaterialIndex id) = 0;
	virtual gpu::BufferHandle getMaterialUniformBuffer() = 0;
	
	virtual TransientPool& getTransientPool() = 0;
	virtual UniformPool& getUniformPool() = 0;
	
	virtual gpu::BufferHandle getInstancedMeshesBuffer() = 0;
	virtual gpu::BufferHandle createBuffer(const MemRef& memory, gpu::BufferFlags flags, const char* debug_name) = 0;
	virtual gpu::TextureHandle createTexture(u32 w, u32 h, u32 depth, gpu::TextureFormat format, gpu::TextureFlags flags, const MemRef& memory, const char* debug_name) = 0;

	virtual RenderBufferHandle createRenderbuffer(const RenderbufferDesc& desc) = 0;
	virtual void releaseRenderbuffer(RenderBufferHandle idx) = 0;
	virtual void setRenderTargets(Span<const RenderBufferHandle> renderbuffers, RenderBufferHandle ds = INVALID_RENDERBUFFER, gpu::FramebufferFlags flags = gpu::FramebufferFlags::NONE) = 0;
	virtual gpu::TextureHandle toTexture(RenderBufferHandle handle) = 0;

	virtual gpu::ProgramHandle queueShaderCompile(struct Shader& shader, const struct ShaderKey& key, gpu::VertexDecl decl) = 0;
	virtual DrawStream& getDrawStream() = 0;
	virtual DrawStream& getEndFrameDrawStream() = 0;

	template <typename T> void pushJob(const char* name, const T& func);

	virtual void beginProfileBlock(const char* name, i64 link, bool stats = false) = 0;
	virtual void endProfileBlock() = 0;

protected:
	virtual void setupJob(void* user_ptr, void(*task)(void*)) = 0;
}; 

struct UniformPool;
struct TransientPool;

LUMIX_RENDERER_API TransientSlice alloc(UniformPool& pool, u32 size);
LUMIX_RENDERER_API TransientSlice alloc(UniformPool& pool, const void* data, u32 size);
LUMIX_RENDERER_API TransientSlice alloc(TransientPool& pool, u32 size);

template <typename T>
void Renderer::pushJob(const char* name, const T& func) {
	struct Context {
		Context(DrawStream& stream, T func, const char* name) 
			: stream(stream)
			, func(func)
			, name(name)
		{}

		static void run(void* ptr) {
			Context* that = (Context*)ptr;
			if (that->name) {
				profiler::beginBlock(that->name);
				const i64 link = profiler::createNewLinkID();
				profiler::link(link);
				profiler::blockColor(Color(0x7f, 0, 0x7f, 0xff).abgr());
				that->stream.beginProfileBlock(that->name, link, false);
			}
			that->func(that->stream);
			if (that->name) {
				that->stream.endProfileBlock();
				profiler::endBlock();
			}
			that->~Context();
		}

		DrawStream& stream;
		T func;
		const char* name;
	};
	
	DrawStream& parent = getDrawStream();
	u8* mem = parent.userAlloc(sizeof(Context));
	DrawStream& stream = parent.createSubstream();
	Context* ctx = new (NewPlaceholder(), mem) Context(stream, func, name);
	setupJob(ctx, &Context::run);
}

} // namespace Lumix

