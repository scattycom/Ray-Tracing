#include "Walnut/Application.h"
#include "Walnut/EntryPoint.h"

#include "Walnut/Image.h"
#include "Walnut/Random.h"
#include "Walnut/Timer.h"

using namespace Walnut;

class ExampleLayer : public Walnut::Layer
{
public:
	virtual void OnUIRender() override
	{
		ImGui::Begin("Settings");

		ImGui::Text("Last Render Time: %.3fms", m_lastRenderTime);
		if (ImGui::Button("Render"))
		{
			Render();
		}

		ImGui::End();

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("ViewPort");

		m_viewportwidth = ImGui::GetContentRegionAvail().x;
		m_viewportheight = ImGui::GetContentRegionAvail().y;

		if(m_image)
			ImGui::Image(m_image->GetDescriptorSet(), { (float)m_image->GetWidth(), (float)m_image->GetHeight() });

		ImGui::End();
		ImGui::PopStyleVar();

		Render();
	}

	void Render()
	{
		Timer timer;

		if (!m_image|| m_viewportwidth !=m_image->GetWidth()||m_viewportheight!=m_image->GetHeight())
		{
			m_image = std::make_shared<Image>(m_viewportwidth, m_viewportheight, ImageFormat::RGBA);
			delete[] m_imageData;
			m_imageData = new uint32_t[m_viewportwidth * m_viewportheight];
		}

		for (uint32_t i = 0; i < m_viewportheight * m_viewportwidth; i++)
		{
			m_imageData[i] = Random::UInt();
			m_imageData[i] |= 0xff000000;
		}

		m_image->SetData(m_imageData);

		m_lastRenderTime = timer.ElapsedMillis();
	}

private:
	std::shared_ptr<Image> m_image;
	uint32_t* m_imageData = nullptr;
	uint32_t m_viewportwidth = 0, m_viewportheight = 0;

	float m_lastRenderTime = 0.0f;

};

Walnut::Application* Walnut::CreateApplication(int argc, char** argv)
{
	Walnut::ApplicationSpecification spec;
	spec.Name = "Ray";

	Walnut::Application* app = new Walnut::Application(spec);
	app->PushLayer<ExampleLayer>();
	app->SetMenubarCallback([app]()
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Exit"))
			{
				app->Close();
			}
			ImGui::EndMenu();
		}
	});
	return app;
}