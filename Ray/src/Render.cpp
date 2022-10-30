#include"render.h"

void Renderer::OnResize(uint32_t width, uint32_t height)
{
	if (m_FinalImage)
	{
		if (m_FinalImage->GetWidth() == width && m_FinalImage->GetHeight() == height)
			return;

		m_FinalImage->Resize(width, height);
	}
	else
	{
		m_FinalImage = std::make_shared<Walnut::Image>(width, height, Walnut::ImageFormat::RGBA);
	}

	delete[] m_imageData;
	m_imageData = new uint32_t[width * height];

}

void Renderer::render()
{
	//render every pixel

	for (uint32_t y = 0; y < m_FinalImage->GetHeight(); y++)
	{
		for (uint32_t x = 0; x < m_FinalImage->GetWidth() ; x++)
		{
			glm::vec2 coord = { x / (float)m_FinalImage->GetWidth() ,y / (float)m_FinalImage->GetHeight() };
			coord = coord * 2.0f - 1.0f;//-1---1

			m_imageData[x + y * m_FinalImage->GetWidth()] = Perpixel(coord);
			
		}
	}
	m_FinalImage->SetData(m_imageData);
}

uint32_t Renderer::Perpixel(glm::vec2 coord)
{
	uint8_t r = (uint8_t)(coord.x * 255.0f);
	uint8_t g = (uint8_t)(coord.y * 255.0f);

	glm::vec3 rayOrigin (0.0f, 0.0f, 2.0f);
	glm::vec3 rayDirection = { coord.x, coord.y , -1.0f };
	//glm::vec3 rayDirection = raySrceen - rayOrigin;
	float radius = 0.5f;

	float a = glm::dot(rayDirection, rayDirection);
	float b = 2.0f * glm::dot(rayOrigin, rayDirection);
	float c = glm::dot(rayOrigin, rayOrigin) - radius * radius;

	float dis = b * b - 4.0f * a * c;

	if (dis >= 0.0f)
		return 0xffff00ff;


	return 0xff000000 ;
}
