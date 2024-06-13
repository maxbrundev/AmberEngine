#include "Amberpch.h"

#include "AmberEngine/UI/Widgets/TextDropTarget.h"

#include "AmberEngine/Managers/ModelManager.h"
#include "AmberEngine/Tools/Global/ServiceLocator.h"
#include "AmberEngine/Tools/Utils/PathParser.h"

AmberEngine::UI::Widgets::TextDropTarget::TextDropTarget(const std::string& p_content, Eventing::Event<>* p_updateNotifier) : DataWidget(content), content(p_content), UpdateNotif(p_updateNotifier)
{
}

void AmberEngine::UI::Widgets::TextDropTarget::Update()
{
	if (ImGui::BeginDragDropTarget())
	{
		__debugbreak();
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("File"))
		{
			std::string data = *static_cast<std::string*>(payload->Data);

			if (Tools::Utils::PathParser::GetFileType(data) == Tools::Utils::PathParser::EFileType::MODEL)
			{
				if (auto* resource = Tools::Global::ServiceLocator::Get<ResourceManagement::ModelManager>().GetResource(data); resource)
				{
					if (UpdateNotif)
						UpdateNotif->Invoke();

					content = data;
				}
			}
		}

		ImGui::EndDragDropTarget();
	}
}

void AmberEngine::UI::Widgets::TextDropTarget::DrawImplementation()
{
	ImGui::Text(content.c_str());

	Update();
}
