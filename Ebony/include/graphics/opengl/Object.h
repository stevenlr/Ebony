#ifndef EBONY_GRAPHICS_OPENGL_OBJECT_H_
#define EBONY_GRAPHICS_OPENGL_OBJECT_H_

#include <glad/glad.h>

#include "enums.h"

namespace ebony { namespace gl {

struct BufferTraits {
	typedef GLuint ValueType;
	static const ValueType nullValue = 0;

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
	typedef GLuint ValueType;
	static const ValueType nullValue = 0;

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
	typedef GLuint ValueType;
	static const ValueType nullValue = 0;

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
	typedef GLuint ValueType;
	static const ValueType nullValue = 0;

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
	typedef GLuint ValueType;
	static const ValueType nullValue = 0;

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
	typedef GLuint ValueType;
	static const ValueType nullValue = 0;

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
	typedef GLuint ValueType;
	static const ValueType nullValue = 0;

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
	typedef GLuint ValueType;
	static const ValueType nullValue = 0;

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
	typedef GLuint ValueType;
	static const ValueType nullValue = 0;

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
	Object(const Object<T> &obj) : _obj(obj._obj) {}
	~Object() { T::destroy(_obj); }

	operator typename T::ValueType() const { return _obj; }
	operator bool() const { return _obj != T::nullValue; }

	Object<T> &operator=(const Object<T> &obj)
	{
		_obj = obj;
		return *this;
	}

private:
	typename T::ValueType _obj;
};
	
template<>
class Object<ShaderTraits> {
public:
	Object(ShaderType type) : _obj(ShaderTraits::create(type)) {}
	Object(const Object<ShaderTraits> &obj) : _obj(obj._obj) {}
	~Object() { ShaderTraits::destroy(_obj); }

	operator ShaderTraits::ValueType() const { return _obj; }
	operator bool() const { return _obj != ShaderTraits::nullValue; }

	Object<ShaderTraits> &operator=(const Object<ShaderTraits> &obj)
	{
		_obj = obj._obj;
		return *this;
	}

private:
	ShaderTraits::ValueType _obj;
};

typedef Object<BufferTraits> Buffer;
typedef Object<VertexArrayTraits> VertexArray;
typedef Object<FramebufferTraits> Framebuffer;
typedef Object<RenderbufferTraits> Renderbuffer;
typedef Object<TextureTraits> Texture;
typedef Object<SamplerTraits> Sampler;
typedef Object<QueryTraits> Query;
typedef Object<ProgramTraits> Program;
typedef Object<ShaderTraits> Shader;

}}

#endif
