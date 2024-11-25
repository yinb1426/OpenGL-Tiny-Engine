#include <glad/glad.h>
namespace TinyEngine
{
	enum class BufferType
	{
		Vertex,
		Index,
		Uniform,
		Other
	};

	enum class DataUsage
	{
		Static,
		Dynamic,
		Stream
	};

	class Buffer
	{
	public:
		Buffer(BufferType type, DataUsage usage, const void* data, size_t size) 
		{
			CreateBuffer(type, usage, data, size);
		}

		~Buffer() 
		{
			DeleteBuffer();
		}

		void CreateBuffer(BufferType type, DataUsage usage, const void* data, size_t size)
		{
			this->type = GetBufferType(type);
			this->usage = GetDataUsage(usage);
			if (ID == 0)
				glGenBuffers(1, &this->ID);
			glBindBuffer(this->type, this->ID);
			glBufferData(this->type, size, data, this->usage);
		}

		void UpdateBuffer(const void* data, size_t size, size_t offset)
		{
			glBindBuffer(this->type, this->ID);
			glBufferSubData(this->type, offset, size, data);
			glBindBuffer(this->type, 0);
		}

		void Bind()
		{
			glBindBuffer(this->type, this->ID);
		}

		void BindToBindingPoint(unsigned int bindingPoint)
		{
			if (this->type == GL_UNIFORM_BUFFER)
				glBindBufferBase(this->type, bindingPoint, this->ID);
			else
				throw std::invalid_argument("Buffer Type must be Uniform!");
		}

		void UnbindFromBindingPoint(unsigned int bindingPoint)
		{
			if (this->type == GL_UNIFORM_BUFFER)
				glBindBufferBase(this->type, bindingPoint, 0);
			else
				throw std::invalid_argument("Buffer Type must be Uniform!");
		}

		void Unbind()
		{
			glBindBuffer(this->type, 0);
		}

		void DeleteBuffer()
		{
			if (this->ID != 0)
			{
				glDeleteBuffers(1, &this->ID);
				this->ID = 0;
			}
		}
	private:
		GLenum GetBufferType(BufferType type) const
		{
			switch (type) 
			{
				case(BufferType::Vertex):	return GL_ARRAY_BUFFER;
				case(BufferType::Index):	return GL_ELEMENT_ARRAY_BUFFER;
				case(BufferType::Uniform):	return GL_UNIFORM_BUFFER;
				default:					return GL_ARRAY_BUFFER;
			}

		}
		GLenum GetDataUsage(DataUsage usage) const
		{
			switch (usage)
			{
				case(DataUsage::Static):	return GL_STATIC_DRAW;
				case(DataUsage::Dynamic):	return GL_DYNAMIC_DRAW;
				case(DataUsage::Stream):	return GL_STREAM_DRAW;
				default:					return GL_STATIC_DRAW;
			}
		}
		unsigned int ID = 0;
		GLenum type;
		GLenum usage;
	};
}