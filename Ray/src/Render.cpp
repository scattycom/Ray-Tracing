#include"render.h"

namespace utils
{
	static uint32_t ConvertToRGBA(const glm::vec4 color)
	{
		uint8_t r = uint8_t(color.r * 255.0f);
		uint8_t g = uint8_t(color.g * 255.0f);
		uint8_t b = uint8_t(color.b * 255.0f);
		uint8_t a = uint8_t(color.a * 255.0f);

		uint32_t result = (a << 24) | (b << 16) | (g << 8) | r;

		return result;
	}
}


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

	float aspectRatio = m_FinalImage->GetWidth() / (float)m_FinalImage->GetHeight();

	for (uint32_t y = 0; y < m_FinalImage->GetHeight(); y++)
	{
		for (uint32_t x = 0; x < m_FinalImage->GetWidth() ; x++)
		{
			glm::vec2 coord = { x / (float)m_FinalImage->GetWidth() ,y / (float)m_FinalImage->GetHeight() };

			coord = coord * 2.0f - 1.0f;//-1---1
			glm::vec4 color = Perpixel(coord);
			color = glm::clamp(color, glm::vec4(0.0f), glm::vec4(1.0f));

			m_imageData[x + y * m_FinalImage->GetWidth()] = utils::ConvertToRGBA(color);
		}
	}
	m_FinalImage->SetData(m_imageData);
}

glm::vec4 Renderer::Perpixel(glm::vec2 coord)
{
	uint8_t r = (uint8_t)(coord.x * 255.0f);
	uint8_t g = (uint8_t)(coord.y * 255.0f);


	glm::vec3 rayOrigin(0.0f, 0.0f, 1.0f);
	glm::vec3 rayDirection = { coord.x, coord.y , -1.0f };
	//glm::vec3 rayDirection = raySrceen - rayOrigin;
	float radius = 0.5f;

	float a = glm::dot(rayDirection, rayDirection);
	float b = 2.0f * glm::dot(rayOrigin, rayDirection);
	float c = glm::dot(rayOrigin, rayOrigin) - radius * radius;

	float dis = b * b - 4.0f * a * c;

	if (dis < 0.0f)
		return glm::vec4(0, 0, 0, 1);

	float t0 = (-b + glm::sqrt(dis)) / (2.0f * a);
	float closeT = (-b - glm::sqrt(dis)) / (2.0f * a);

	glm::vec3 hitpoint = rayOrigin + rayDirection * closeT;
	glm::vec3 normal =glm::normalize(hitpoint);

	glm::vec3 lightdirection =glm::normalize(glm::vec3(-1.0f, -1.0f, -1.0f));

	float d = glm::max(glm::dot(normal, -lightdirection), 0.0f);

	glm::vec3 sphereColor(1, 0, 1);
	sphereColor *= d;
	return glm::vec4(sphereColor, 1);

}
