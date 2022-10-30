#include "Walnut/Application.h"
#include "Walnut/EntryPoint.h"

#include "Walnut/Image.h"
#include "Walnut/Random.h"
#include "Walnut/Timer.h"
#include "Render.h"

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

		auto image = m_renderer.GetFinalImage();
		if (m_renderer.GetFinalImage())
			ImGui::Image(image->GetDescriptorSet(), { (float)image->GetWidth(), (float)image->GetHeight() }
				, ImVec2{ 0,1 }, ImVec2{ 1,0 });

		ImGui::End();
		ImGui::PopStyleVar();

		Render();
	}

	void Render()
	{
		Timer timer;

		m_renderer.OnResize(m_viewportwidth, m_viewportheight);
		m_renderer.render();

		m_lastRenderTime = timer.ElapsedMillis();
	}

private:
	Renderer m_renderer;

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