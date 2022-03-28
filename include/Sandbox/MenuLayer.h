#pragma once
#include "Layer.h"

#include <vector>
#include <string.h>
#include <functional>
#include <iostream>

class MenuLayer : public Layer
{
public:
	MenuLayer(Layer*& currentLayerPointer);
	
	void OnUpdate(float delta) override;
	void OnImGuiRender() override;

	template <typename T>
	void RegisterLayer(const std::string& name)
	{
		std::cout << "Register test " << name << std::endl;
		m_Layers.push_back(std::make_pair(name, []() { return new T(); }));
	}

private:
	Layer*& m_CurrentLayer;
	std::vector<std::pair<std::string, std::function<Layer* ()>>> m_Layers;
};