#pragma once

#include"Walnut/Image.h"
#include"Walnut/Random.h"

#include<memory>

class Renderer
{
public:
	Renderer() = default;

	void OnResize(uint32_t width, uint32_t height);
	void render();

	std::shared_ptr<Walnut::Image> GetFinalImage() const { return m_FinalImage; }
private:
	uint32_t Perpixel(glm::vec2 coord);

private:
	std::shared_ptr<Walnut::Image> m_FinalImage;
	uint32_t* m_imageData = nullptr;

};

