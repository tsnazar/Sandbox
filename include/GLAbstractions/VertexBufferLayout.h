#pragma once
#include <string>
#include <vector>
#include <glad/glad.h>

#include "Core.h"

enum class VertexBufferElementType
{
	None = 0, Float, Float2, Float3, Float4,
	Mat3, Mat4,
	Int, Int2, Int3, Int4
};

static unsigned int VertexBufferElementTypeSize(const VertexBufferElementType& type)
{
	switch (type)
	{
	case VertexBufferElementType::Float:  return 4;
	case VertexBufferElementType::Float2: return 2 * 4;
	case VertexBufferElementType::Float3: return 3 * 4;
	case VertexBufferElementType::Float4: return 4 * 4;
	case VertexBufferElementType::Mat3:   return 3 * 3 * 4;
	case VertexBufferElementType::Mat4:	  return 4 * 4 * 4;
	case VertexBufferElementType::Int:    return 4;
	case VertexBufferElementType::Int2:   return 2 * 4;
	case VertexBufferElementType::Int3:   return 3 * 4;
	case VertexBufferElementType::Int4:   return 4 * 4;
	}
	SANDBOX_ASSERT(false);
	return 0;
}

static GLenum BufferElementTypeToOpenGLType(const VertexBufferElementType& type)
{
	switch (type)
	{
	case VertexBufferElementType::Float:  return GL_FLOAT;
	case VertexBufferElementType::Float2: return GL_FLOAT;
	case VertexBufferElementType::Float3: return GL_FLOAT;
	case VertexBufferElementType::Float4: return GL_FLOAT;
	case VertexBufferElementType::Mat3:   return GL_FLOAT;
	case VertexBufferElementType::Mat4:	  return GL_FLOAT;
	case VertexBufferElementType::Int:    return GL_INT;
	case VertexBufferElementType::Int2:   return GL_INT;
	case VertexBufferElementType::Int3:   return GL_INT;
	case VertexBufferElementType::Int4:   return GL_INT;
	}
	SANDBOX_ASSERT(false);
	return 0;
}

struct VertexBufferElement
{
	VertexBufferElementType Type;
	std::string Name;
	unsigned int Offset;
	unsigned int Size;

	VertexBufferElement(const VertexBufferElementType& type, const std::string& name)
		: Type(type), Name(name), Offset(0), Size(VertexBufferElementTypeSize(type)) {}

	unsigned int GetElementCount() const
	{
		switch (Type) {
		case VertexBufferElementType::Float:  return 1;
		case VertexBufferElementType::Float2: return 2;
		case VertexBufferElementType::Float3: return 3;
		case VertexBufferElementType::Float4: return 4;
		case VertexBufferElementType::Mat3:   return 3 * 3;
		case VertexBufferElementType::Mat4:	  return 4 * 4;
		case VertexBufferElementType::Int:    return 1;
		case VertexBufferElementType::Int2:   return 2;
		case VertexBufferElementType::Int3:   return 3;
		case VertexBufferElementType::Int4:   return 4;
		}
		SANDBOX_ASSERT(false);
		return 0;
	}
};

class VertexBufferLayout
{
public:
	
	VertexBufferLayout() {}
	VertexBufferLayout(const std::initializer_list<VertexBufferElement>& elements)
		: m_Elements(elements) 
	{
		CalculateElementsOffset();
	}

	const std::vector<VertexBufferElement>& GetElements() const { return m_Elements; }
	const unsigned int GetStride() const { return m_Stride; }

	std::vector<VertexBufferElement>::iterator begin() { return m_Elements.begin(); }
	std::vector<VertexBufferElement>::iterator end() { return m_Elements.end(); }

	const std::vector<VertexBufferElement>::const_iterator begin() const { return m_Elements.begin(); }
	const std::vector<VertexBufferElement>::const_iterator end() const { return m_Elements.end(); }

private:
	void CalculateElementsOffset()
	{
		unsigned int offset = 0;
		m_Stride = 0;
		for (auto& element : m_Elements)
		{
			element.Offset = offset;
			offset += element.Size;
			m_Stride += element.Size;
		}
	}
private:
	std::vector<VertexBufferElement> m_Elements;
	unsigned int m_Stride = 0;
};
