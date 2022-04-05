#pragma once
#include "Buffer.h"

#include <vector>
#include <memory>

class VertexArray
{
public:
	VertexArray();
	~VertexArray();

	void Bind() const;
	void UnBind() const;

	void AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer);
	void SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer);

	const IndexBuffer& GetIndexBuffer() const { return *m_IndexBuffer; }

private:
	unsigned int m_RendererID;
	std::vector<std::shared_ptr<VertexBuffer>> m_VertexBuffers;
	std::shared_ptr<IndexBuffer> m_IndexBuffer;
};