#ifndef EBONY_GRAPHICS_OPENGL_OBJECT_H_
#define EBONY_GRAPHICS_OPENGL_OBJECT_H_

#include <glad/glad.h>

#include "enums.h"

namespace ebony { namespace gl {

struct BufferTraits {
	using ValueType = GLuint;
	static constexpr ValueType nullValue = 0;

	static ValueType create()
	{
		ValueType object;
		glGenBuffers(1, &object);
		return object;
	}

	static void destroy(ValueType object)
	{
		glDeleteBuffers(1, &object);
	}
};

struct VertexArrayTraits {
	using ValueType = GLuint;
	static constexpr ValueType nullValue = 0;

	static ValueType create()
	{
		ValueType object;
		glGenVertexArrays(1, &object);
		return object;
	}

	static void destroy(ValueType object)
	{
		glDeleteVertexArrays(1, &object);
	}
};

struct FramebufferTraits {
	using ValueType = GLuint;
	static constexpr ValueType nullValue = 0;

	static ValueType create()
	{
		ValueType object;
		glGenFramebuffers(1, &object);
		return object;
	}

	static void destroy(ValueType object)
	{
		glDeleteFramebuffers(1, &object);
	}
};

struct RenderbufferTraits {
	using ValueType = GLuint;
	static constexpr ValueType nullValue = 0;

	static ValueType create()
	{
		ValueType object;
		glGenRenderbuffers(1, &object);
		return object;
	}

	static void destroy(ValueType object)
	{
		glDeleteRenderbuffers(1, &object);
	}
};

struct TextureTraits {
	using ValueType = GLuint;
	static constexpr ValueType nullValue = 0;

	static ValueType create()
	{
		ValueType object;
		glGenTextures(1, &object);
		return object;
	}

	static void destroy(ValueType object)
	{
		glDeleteTextures(1, &object);
	}
};

struct SamplerTraits {
	using ValueType = GLuint;
	static constexpr ValueType nullValue = 0;

	static ValueType create()
	{
		ValueType object;
		glGenSamplers(1, &object);
		return object;
	}

	static void destroy(ValueType object)
	{
		glDeleteSamplers(1, &object);
	}
};

struct QueryTraits {
	using ValueType = GLuint;
	static constexpr ValueType nullValue = 0;

	static ValueType create()
	{
		ValueType object;
		glGenQueries(1, &object);
		return object;
	}

	static void destroy(ValueType object)
	{
		glDeleteQueries(1, &object);
	}
};

struct ProgramTraits {
	using ValueType = GLuint;
	static constexpr ValueType nullValue = 0;

	static ValueType create()
	{
		return glCreateProgram();
	}

	static void destroy(ValueType object)
	{
		glDeleteProgram(object);
	}
};

struct ShaderTraits {
	using ValueType = GLuint;
	static constexpr ValueType nullValue = 0;

	static ValueType create(ShaderType type)
	{
		return glCreateShader(type);
	}

	static void destroy(ValueType object)
	{
		glDeleteShader(object);
	}
};

template<typename T>
class Object {
public:
	Object() : _obj(T::create()) {}
	~Object() { T::destroy(_obj); }

	constexpr operator typename T::ValueType() const { return _obj; }
	constexpr operator bool() const { return _obj != T::nullValue; }

	Object(const Object<T> &) = delete;
	Object<T> &operator=(const Object<T> &) = delete;

private:
	typename T::ValueType _obj;
};
	
template<>
class Object<ShaderTraits> {
public:
	Object(ShaderType type) : _obj(ShaderTraits::create(type)) {}
	~Object() { ShaderTraits::destroy(_obj); }

	operator ShaderTraits::ValueType() const { return _obj; }
	operator bool() const { return _obj != ShaderTraits::nullValue; }

	Object(const Object<ShaderTraits> &obj) = delete;
	Object<ShaderTraits> &operator=(const Object<ShaderTraits> &obj) =	delete;

private:
	ShaderTraits::ValueType _obj;
};

using Buffer			= Object<BufferTraits>;
using VertexArray		= Object<VertexArrayTraits>;
using Framebuffer		= Object<FramebufferTraits>;
using Renderbuffer		= Object<RenderbufferTraits>;
using Texture			= Object<TextureTraits>;
using Sampler			= Object<SamplerTraits>;
using Query				= Object<QueryTraits>;
using Program			= Object<ProgramTraits>;
using Shader			= Object<ShaderTraits>;

}}

#endif
