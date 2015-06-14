#ifndef EBONY_GRAPHICS_OPENGL_OPENGL_OBJECT_H_
#define EBONY_GRAPHICS_OPENGL_OPENGL_OBJECT_H_

#include <GL/glew.h>

#include "Enums.h"

namespace ebony { namespace gl {

	struct BufferTraits {
		typedef GLuint value_type;
		static const value_type null_value = 0;

		static value_type create()
		{
			value_type object;
			glGenBuffers(1, &object);
			return object;
		}

		static void destroy(value_type object)
		{
			glDeleteBuffers(1, &object);
		}
	};

	struct VertexArrayTraits {
		typedef GLuint value_type;
		static const value_type null_value = 0;

		static value_type create()
		{
			value_type object;
			glGenVertexArrays(1, &object);
			return object;
		}

		static void destroy(value_type object)
		{
			glDeleteVertexArrays(1, &object);
		}
	};

	struct FramebufferTraits {
		typedef GLuint value_type;
		static const value_type null_value = 0;

		static value_type create()
		{
			value_type object;
			glGenFramebuffers(1, &object);
			return object;
		}

		static void destroy(value_type object)
		{
			glDeleteFramebuffers(1, &object);
		}
	};

	struct RenderbufferTraits {
		typedef GLuint value_type;
		static const value_type null_value = 0;

		static value_type create()
		{
			value_type object;
			glGenRenderbuffers(1, &object);
			return object;
		}

		static void destroy(value_type object)
		{
			glDeleteRenderbuffers(1, &object);
		}
	};

	struct TextureTraits {
		typedef GLuint value_type;
		static const value_type null_value = 0;

		static value_type create()
		{
			value_type object;
			glGenTextures(1, &object);
			return object;
		}

		static void destroy(value_type object)
		{
			glDeleteTextures(1, &object);
		}
	};

	struct SamplerTraits {
		typedef GLuint value_type;
		static const value_type null_value = 0;

		static value_type create()
		{
			value_type object;
			glGenSamplers(1, &object);
			return object;
		}

		static void destroy(value_type object)
		{
			glDeleteSamplers(1, &object);
		}
	};

	struct QueryTraits {
		typedef GLuint value_type;
		static const value_type null_value = 0;

		static value_type create()
		{
			value_type object;
			glGenQueries(1, &object);
			return object;
		}

		static void destroy(value_type object)
		{
			glDeleteQueries(1, &object);
		}
	};

	struct ProgramTraits {
		typedef GLuint value_type;
		static const value_type null_value = 0;

		static value_type create()
		{
			return glCreateProgram();
		}

		static void destroy(value_type object)
		{
			glDeleteProgram(object);
		}
	};

	struct ShaderTraits {
		typedef GLuint value_type;
		static const value_type null_value = 0;

		static value_type create(ShaderType type)
		{
			return glCreateShader(type);
		}

		static void destroy(value_type object)
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

		operator typename T::value_type() const { return _obj; }
		operator bool() const { return _obj != T::null_value; }

		Object<T> &operator=(const Object<T> &obj)
		{
			_obj = obj;
			return *this;
		}

	private:
		typename T::value_type _obj;
	};
	
	template<>
	class Object<ShaderTraits> {
	public:
		Object(ShaderType type) : _obj(ShaderTraits::create(type)) {}
		Object(const Object<ShaderTraits> &obj) : _obj(obj._obj) {}
		~Object() { ShaderTraits::destroy(_obj); }

		operator ShaderTraits::value_type() const { return _obj; }
		operator bool() const { return _obj != ShaderTraits::null_value; }

		Object<ShaderTraits> &operator=(const Object<ShaderTraits> &obj)
		{
			_obj = obj._obj;
			return *this;
		}

	private:
		ShaderTraits::value_type _obj;
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

}
}

#endif
