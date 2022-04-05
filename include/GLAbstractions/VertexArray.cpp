#include "VertexArray.h"

#include "Core.h"


VertexArray::VertexArray()
{
	GLCall(glGenVertexArrays(1, &m_RendererID));
}

VertexArray::~VertexArray()
{
	GLCall(glDeleteVertexArrays(1, &m_RendererID));
}

void VertexArray::Bind() const
{
	GLCall(glBindVertexArray(m_RendererID));
}

void VertexArray::UnBind() const
{
	GLCall(glBindVertexArray(0));
}

void VertexArray::AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer)
{
	GLCall(glBindVertexArray(m_RendererID));
	vertexBuffer->Bind();

	SANDBOX_ASSERT(vertexBuffer->GetLayout().GetElements().size()); // assert if layout is empty

	unsigned int index = 0;
	const auto& layout = vertexBuffer->GetLayout();
	for (const auto& element : layout)
	{
		GLCall(glEnableVertexAttribArray(index));
		GLCall(glVertexAttribPointer(index, element.GetElementCount(), BufferElementTypeToOpenGLType(element.Type), GL_FALSE, layout.GetStride(), (void*)element.Offset));
		index++;
	}
	m_VertexBuffers.push_back(vertexBuffer);
}

void VertexArray::SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer)
{
	GLCall(glBindVertexArray(m_RendererID));
	indexBuffer->Bind();
	m_IndexBuffer = indexBuffer;
}
