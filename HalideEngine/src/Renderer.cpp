#include "Renderer.h"
#include "Walnut/Random.h"

namespace Utils
{
	static uint32_t ConvertToRGBA(const glm::vec4& color) {
		uint8_t r = (color.r * 255.0f);
		uint8_t g = (color.g * 255.0f);
		uint8_t b = (color.b * 255.0f);
		uint8_t a = (color.a * 255.0f);

		uint32_t result = (a << 24) | (b << 16) | (g << 8) | r;
		return result;
	}
}

void Renderer::OnResize(uint16_t width, uint16_t height)
{
	if (m_FinalImage)
	{
		if (m_FinalImage->GetWidth() == width && m_FinalImage->GetHeight() == height)
		{
			return;
		}
		m_FinalImage->Resize(width, height);
	}
	else
	{
		m_FinalImage = std::make_shared<Walnut::Image>(width, height, Walnut::ImageFormat::RGBA);
	}

	delete[] m_ImageData;
	m_ImageData = new uint32_t[width * height];
}

void Renderer::Render(const Camera& camera, float _r, float _g, float _b, float _i)
{ 
	Ray ray;
	ray.Origin = camera.GetPosition();

	for (uint32_t y = 0; y < m_FinalImage->GetHeight(); y++)
	{
		for (uint32_t x = 0; x < m_FinalImage->GetWidth(); x++)
		{
			ray.Direction = camera.GetRayDirections()[x+y* m_FinalImage->GetWidth()];
			glm::vec4 color = TraceRay(ray, _r, _g, _b, _i);
			color = glm::clamp(color, glm::vec4(0.0f), glm::vec4(1.0f));
			m_ImageData[x + y * m_FinalImage->GetWidth()] = Utils::ConvertToRGBA(color);
		}
	}

	m_FinalImage->SetData(m_ImageData);
}

glm::vec4 Renderer::TraceRay(const Ray& ray, float _r, float _g, float _b, float _i)
{
	float radius = 0.5f;
	// rayDirection = glm::normalize(rayDirection);
	
	// 	// (bx^2 + by^2)t^2 + (2(axbx + ayby)) + (ax^2 + ay^2 - r^2) = 0
	// a = ray origin
	// b = ray direction
	// r = ray radius
	// t = hit distance

	float a = glm::dot(ray.Direction, ray.Direction);
	float b = 2.0f * glm::dot(ray.Origin, ray.Direction);
	float c = glm::dot(ray.Origin, ray.Origin) - radius * radius;

	float discriminant = b * b - 4.0f * a * c;

	if (discriminant < 0.0f)
	{
		return glm::vec4(0, 0, 0, 1);
	}

	float t0 = (-b + glm::sqrt(discriminant)) / (2.0f * a);
	float closestT = (-b - glm::sqrt(discriminant)) / (2.0f * a);

	glm::vec3 hitPoint = ray.Origin + ray.Direction * closestT;
	glm::vec3 normal = glm::normalize(hitPoint);

	glm::vec3 lightDir = glm::normalize(glm::vec3( - 1, -1, -1));

	float d = glm::max(glm::dot(normal, -lightDir), 0.0f); //cos(angle)

	glm::vec3 sphereColor(_r, _g, _b);
	sphereColor *= d;
	return glm::vec4(sphereColor, _i); //Color, Intensity(0-1)
}
