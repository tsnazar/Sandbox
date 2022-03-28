#pragma once

#include <string>

#include "Event.h"

class Layer
{
public:
	Layer(const std::string& name = "Layer")
		: m_DebugName(name) {}
	virtual ~Layer() = default;

	virtual void OnAttach() {}
	virtual void OnDetach() {}
	virtual void OnUpdate(float delta) {}
	virtual void OnImGuiRender() {}
	virtual void OnEvent(Event& event) {}

	inline const std::string& GetName() const { return m_DebugName;  }
private:
	std::string m_DebugName;
};
