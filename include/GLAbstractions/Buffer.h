#pragma once
#include "VertexBufferLayout.h"

class VertexBuffer
{
public:
	VertexBuffer(const void* data, const unsigned int size);
	~VertexBuffer();

	void Bind() const;
	void UnBind() const;

	inline const VertexBufferLayout& GetLayout() { return m_Layout; }
	inline void SetLayout(const VertexBufferLayout& layout) { m_Layout = layout; }

private:
	unsigned int m_RendererID;
	VertexBufferLayout m_Layout;
};

class IndexBuffer
{
public:
	IndexBuffer(const void* data, const unsigned int count);
	~IndexBuffer();

	void Bind() const;
	void UnBind() const;

	unsigned int GetCount() const { return m_Count; }

private:
	unsigned int m_RendererID;
	unsigned int m_Count;
};