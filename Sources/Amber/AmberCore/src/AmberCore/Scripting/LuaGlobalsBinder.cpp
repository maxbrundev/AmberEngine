#include "Amberpch.h"

#include "AmberCore/Scripting/LuaGlobalsBinder.h"

#include "AmberDebug/Logger.h"

#include "AmberTools/Global/ServiceLocator.h"
#include "AmberTools/Utils/Random.h"

#include "AmberCore/ECS/Actor.h"
#include "AmberCore/ECS/PhysicsWrapper.h"
#include "AmberCore/SceneSystem/Scene.h"
#include "AmberCore/SceneSystem/SceneManager.h"
#include "AmberCore/ResourceManagement/ModelManager.h"
#include "AmberCore/ResourceManagement/ShaderManager.h"
#include "AmberCore/ResourceManagement/TextureManager.h"
#include "AmberCore/ResourceManagement/MaterialManager.h"
#include "AmberCore/ResourceManagement/SoundManager.h"

#include "AmberWindowing/Context/Window.h"
#include "AmberWindowing/Inputs/InputManager.h"

void AmberCore::Scripting::LuaGlobalsBinder::BindGlobals(sol::state& p_luaState)
{
	auto CreateActorOverload = sol::overload
	(
		sol::resolve<ECS::Actor&(void)>(&SceneSystem::Scene::CreateActor),
		sol::resolve<ECS::Actor&(const std::string&, const std::string&)>(&SceneSystem::Scene::CreateActor)
	);

	p_luaState.new_usertype<SceneSystem::Scene>("Scene",
		"FindActorByName", &SceneSystem::Scene::FindActorByName,
		"FindActorByTag", &SceneSystem::Scene::FindActorByTag,
		"FindActorsByName", &SceneSystem::Scene::FindActorsByName,
		"FindActorsByTag", &SceneSystem::Scene::FindActorsByTag,
		"CreateActor", CreateActorOverload
		);

	p_luaState.new_enum<AmberWindowing::Inputs::EKey>("Key",
		{
			{"UNKNOWN", AmberWindowing::Inputs::EKey::KEY_UNKNOWN},
			{"SPACE", AmberWindowing::Inputs::EKey::KEY_SPACE},
			{"APOSTROPHE", AmberWindowing::Inputs::EKey::KEY_APOSTROPHE},
			{"COMMA", AmberWindowing::Inputs::EKey::KEY_COMMA},
			{"MINUS", AmberWindowing::Inputs::EKey::KEY_MINUS},
			{"PERIOD", AmberWindowing::Inputs::EKey::KEY_PERIOD},
			{"SLASH", AmberWindowing::Inputs::EKey::KEY_SLASH},
			{"0", AmberWindowing::Inputs::EKey::KEY_0},
			{"1", AmberWindowing::Inputs::EKey::KEY_1},
			{"2", AmberWindowing::Inputs::EKey::KEY_2},
			{"3", AmberWindowing::Inputs::EKey::KEY_3},
			{"4", AmberWindowing::Inputs::EKey::KEY_4},
			{"5", AmberWindowing::Inputs::EKey::KEY_5},
			{"6", AmberWindowing::Inputs::EKey::KEY_6},
			{"7", AmberWindowing::Inputs::EKey::KEY_7},
			{"8", AmberWindowing::Inputs::EKey::KEY_8},
			{"9", AmberWindowing::Inputs::EKey::KEY_9},
			{"SEMICOLON", AmberWindowing::Inputs::EKey::KEY_SEMICOLON},
			{"EQUAL", AmberWindowing::Inputs::EKey::KEY_EQUAL},
			{"A", AmberWindowing::Inputs::EKey::KEY_A},
			{"B", AmberWindowing::Inputs::EKey::KEY_B},
			{"C", AmberWindowing::Inputs::EKey::KEY_C},
			{"D", AmberWindowing::Inputs::EKey::KEY_D},
			{"E", AmberWindowing::Inputs::EKey::KEY_E},
			{"F", AmberWindowing::Inputs::EKey::KEY_F},
			{"G", AmberWindowing::Inputs::EKey::KEY_G},
			{"H", AmberWindowing::Inputs::EKey::KEY_H},
			{"I", AmberWindowing::Inputs::EKey::KEY_I},
			{"J", AmberWindowing::Inputs::EKey::KEY_J},
			{"K", AmberWindowing::Inputs::EKey::KEY_K},
			{"L", AmberWindowing::Inputs::EKey::KEY_L},
			{"M", AmberWindowing::Inputs::EKey::KEY_M},
			{"N", AmberWindowing::Inputs::EKey::KEY_N},
			{"O", AmberWindowing::Inputs::EKey::KEY_O},
			{"P", AmberWindowing::Inputs::EKey::KEY_P},
			{"Q", AmberWindowing::Inputs::EKey::KEY_Q},
			{"R", AmberWindowing::Inputs::EKey::KEY_R},
			{"S", AmberWindowing::Inputs::EKey::KEY_S},
			{"T", AmberWindowing::Inputs::EKey::KEY_T},
			{"U", AmberWindowing::Inputs::EKey::KEY_U},
			{"V", AmberWindowing::Inputs::EKey::KEY_V},
			{"W", AmberWindowing::Inputs::EKey::KEY_W},
			{"X", AmberWindowing::Inputs::EKey::KEY_X},
			{"Y", AmberWindowing::Inputs::EKey::KEY_Y},
			{"Z", AmberWindowing::Inputs::EKey::KEY_Z},
			{"LEFT_BRACKET", AmberWindowing::Inputs::EKey::KEY_LEFT_BRACKET},
			{"BACKSLASH", AmberWindowing::Inputs::EKey::KEY_BACKSLASH},
			{"RIGHT_BRACKET", AmberWindowing::Inputs::EKey::KEY_RIGHT_BRACKET},
			{"GRAVE_ACCENT", AmberWindowing::Inputs::EKey::KEY_GRAVE_ACCENT},
			{"WORLD_1", AmberWindowing::Inputs::EKey::KEY_WORLD_1},
			{"WORLD_2", AmberWindowing::Inputs::EKey::KEY_WORLD_2},
			{"ESCAPE", AmberWindowing::Inputs::EKey::KEY_ESCAPE},
			{"ENTER", AmberWindowing::Inputs::EKey::KEY_ENTER},
			{"TAB", AmberWindowing::Inputs::EKey::KEY_TAB},
			{"BACKSPACE", AmberWindowing::Inputs::EKey::KEY_BACKSPACE},
			{"INSERT", AmberWindowing::Inputs::EKey::KEY_INSERT},
			{"DELETE", AmberWindowing::Inputs::EKey::KEY_DELETE},
			{"RIGHT", AmberWindowing::Inputs::EKey::KEY_RIGHT},
			{"LEFT", AmberWindowing::Inputs::EKey::KEY_LEFT},
			{"DOWN", AmberWindowing::Inputs::EKey::KEY_DOWN},
			{"UP", AmberWindowing::Inputs::EKey::KEY_UP},
			{"PAGE_UP", AmberWindowing::Inputs::EKey::KEY_PAGE_UP},
			{"PAGE_DOWN", AmberWindowing::Inputs::EKey::KEY_PAGE_DOWN},
			{"HOME", AmberWindowing::Inputs::EKey::KEY_HOME},
			{"END", AmberWindowing::Inputs::EKey::KEY_END},
			{"CAPS_LOCK", AmberWindowing::Inputs::EKey::KEY_CAPS_LOCK},
			{"SCROLL_LOCK", AmberWindowing::Inputs::EKey::KEY_SCROLL_LOCK},
			{"NUM_LOCK", AmberWindowing::Inputs::EKey::KEY_NUM_LOCK},
			{"PRINT_SCREEN", AmberWindowing::Inputs::EKey::KEY_PRINT_SCREEN},
			{"PAUSE", AmberWindowing::Inputs::EKey::KEY_PAUSE},
			{"F1", AmberWindowing::Inputs::EKey::KEY_F1},
			{"F2", AmberWindowing::Inputs::EKey::KEY_F2},
			{"F3", AmberWindowing::Inputs::EKey::KEY_F3},
			{"F4", AmberWindowing::Inputs::EKey::KEY_F4},
			{"F5", AmberWindowing::Inputs::EKey::KEY_F5},
			{"F6", AmberWindowing::Inputs::EKey::KEY_F6},
			{"F7", AmberWindowing::Inputs::EKey::KEY_F7},
			{"F8", AmberWindowing::Inputs::EKey::KEY_F8},
			{"F9", AmberWindowing::Inputs::EKey::KEY_F9},
			{"F10", AmberWindowing::Inputs::EKey::KEY_F10},
			{"F11", AmberWindowing::Inputs::EKey::KEY_F11},
			{"F12", AmberWindowing::Inputs::EKey::KEY_F12},
			{"KP_0", AmberWindowing::Inputs::EKey::KEY_KP_0},
			{"KP_1", AmberWindowing::Inputs::EKey::KEY_KP_1},
			{"KP_2", AmberWindowing::Inputs::EKey::KEY_KP_2},
			{"KP_3", AmberWindowing::Inputs::EKey::KEY_KP_3},
			{"KP_4", AmberWindowing::Inputs::EKey::KEY_KP_4},
			{"KP_5", AmberWindowing::Inputs::EKey::KEY_KP_5},
			{"KP_6", AmberWindowing::Inputs::EKey::KEY_KP_6},
			{"KP_7", AmberWindowing::Inputs::EKey::KEY_KP_7},
			{"KP_8", AmberWindowing::Inputs::EKey::KEY_KP_8},
			{"KP_9", AmberWindowing::Inputs::EKey::KEY_KP_9},
			{"KP_DECIMAL", AmberWindowing::Inputs::EKey::KEY_KP_DECIMAL},
			{"KP_DIVIDE", AmberWindowing::Inputs::EKey::KEY_KP_DIVIDE},
			{"KP_MULTIPLY", AmberWindowing::Inputs::EKey::KEY_KP_MULTIPLY},
			{"KP_SUBTRACT", AmberWindowing::Inputs::EKey::KEY_KP_SUBTRACT},
			{"KP_ADD", AmberWindowing::Inputs::EKey::KEY_KP_ADD},
			{"KP_ENTER", AmberWindowing::Inputs::EKey::KEY_KP_ENTER},
			{"KP_EQUAL", AmberWindowing::Inputs::EKey::KEY_KP_EQUAL},
			{"LEFT_SHIFT", AmberWindowing::Inputs::EKey::KEY_LEFT_SHIFT},
			{"LEFT_CONTROL", AmberWindowing::Inputs::EKey::KEY_LEFT_CONTROL},
			{"LEFT_ALT", AmberWindowing::Inputs::EKey::KEY_LEFT_ALT},
			{"LEFT_SUPER", AmberWindowing::Inputs::EKey::KEY_LEFT_SUPER},
			{"RIGHT_SHIFT", AmberWindowing::Inputs::EKey::KEY_RIGHT_SHIFT},
			{"RIGHT_CONTROL", AmberWindowing::Inputs::EKey::KEY_RIGHT_CONTROL},
			{"RIGHT_ALT", AmberWindowing::Inputs::EKey::KEY_RIGHT_ALT},
			{"RIGHT_SUPER", AmberWindowing::Inputs::EKey::KEY_RIGHT_SUPER},
			{"MENU", AmberWindowing::Inputs::EKey::KEY_MENU}
		});

	p_luaState.new_enum<AmberWindowing::Inputs::EMouseButton>("MouseButton",
		{
			{"BUTTON_1", AmberWindowing::Inputs::EMouseButton::MOUSE_BUTTON_1},
			{"BUTTON_2", AmberWindowing::Inputs::EMouseButton::MOUSE_BUTTON_2},
			{"BUTTON_3", AmberWindowing::Inputs::EMouseButton::MOUSE_BUTTON_3},
			{"BUTTON_4", AmberWindowing::Inputs::EMouseButton::MOUSE_BUTTON_4},
			{"BUTTON_5", AmberWindowing::Inputs::EMouseButton::MOUSE_BUTTON_5},
			{"BUTTON_6", AmberWindowing::Inputs::EMouseButton::MOUSE_BUTTON_6},
			{"BUTTON_7", AmberWindowing::Inputs::EMouseButton::MOUSE_BUTTON_7},
			{"BUTTON_8", AmberWindowing::Inputs::EMouseButton::MOUSE_BUTTON_8},
			{"BUTTON_LEFT", AmberWindowing::Inputs::EMouseButton::MOUSE_BUTTON_LEFT},
			{"BUTTON_RIGHT", AmberWindowing::Inputs::EMouseButton::MOUSE_BUTTON_RIGHT},
			{"BUTTON_MIDDLE", AmberWindowing::Inputs::EMouseButton::MOUSE_BUTTON_MIDDLE}
		});

	p_luaState.create_named_table("Debug");
	p_luaState.create_named_table("Inputs");
	p_luaState.create_named_table("Scenes");
	p_luaState.create_named_table("Resources");
	p_luaState.create_named_table("Math");
	p_luaState.create_named_table("Physics");

	p_luaState["Debug"]["Log"] = [](const std::string& p_message) { AB_LOG(p_message); };
	p_luaState["Debug"]["LogInfo"] = [](const std::string& p_message) { AB_LOG_INFO(p_message); };
	p_luaState["Debug"]["LogWarning"] = [](const std::string& p_message) { AB_LOG_WARNING(p_message); };
	p_luaState["Debug"]["LogError"] = [](const std::string& p_message) { AB_LOG_ERROR(p_message); };

	p_luaState["Inputs"]["GetKeyDown"] = [](AmberWindowing::Inputs::EKey p_key) { return AmberTools::Global::ServiceLocator::Get<AmberWindowing::Inputs::InputManager>().IsKeyPressed(p_key); };
	p_luaState["Inputs"]["GetKeyUp"] = [](AmberWindowing::Inputs::EKey p_key) { return AmberTools::Global::ServiceLocator::Get<AmberWindowing::Inputs::InputManager>().IsKeyReleased(p_key); };
	p_luaState["Inputs"]["GetKey"] = [](AmberWindowing::Inputs::EKey p_key) { return AmberTools::Global::ServiceLocator::Get<AmberWindowing::Inputs::InputManager>().GetKey(p_key) == AmberWindowing::Inputs::EKeyState::KEY_DOWN; };

	p_luaState["Inputs"]["GetMouseButtonDown"] = [](AmberWindowing::Inputs::EMouseButton p_button) { return AmberTools::Global::ServiceLocator::Get<AmberWindowing::Inputs::InputManager>().IsMouseButtonPressed(p_button); };
	p_luaState["Inputs"]["GetMouseButtonUp"] = [](AmberWindowing::Inputs::EMouseButton p_button) { return AmberTools::Global::ServiceLocator::Get<AmberWindowing::Inputs::InputManager>().IsMouseButtonReleased(p_button); };
	p_luaState["Inputs"]["GetMouseButton"] = [](AmberWindowing::Inputs::EMouseButton p_button) { return AmberTools::Global::ServiceLocator::Get<AmberWindowing::Inputs::InputManager>().GetMouseButton(p_button) == AmberWindowing::Inputs::EMouseButtonState::MOUSE_DOWN; };

	p_luaState["Inputs"]["GetMousePos"] = []()
	{
		auto [mouseX, mouseY] = AmberTools::Global::ServiceLocator::Get<AmberWindowing::Inputs::InputManager>().GetMousePosition();
		return glm::vec2(static_cast<float>(mouseX), static_cast<float>(mouseY));
	};

	p_luaState["Inputs"]["LockMouse"] = []() { AmberTools::Global::ServiceLocator::Get<AmberWindowing::Context::Window>().SetCursorMode(AmberWindowing::Context::ECursorMode::DISABLED); };
	p_luaState["Inputs"]["UnlockMouse"] = []() { AmberTools::Global::ServiceLocator::Get<AmberWindowing::Context::Window>().SetCursorMode(AmberWindowing::Context::ECursorMode::NORMAL); };

	p_luaState["Scenes"]["GetCurrentScene"] = []() { return AmberTools::Global::ServiceLocator::Get<SceneSystem::SceneManager>().GetCurrentScene(); };
	p_luaState["Scenes"]["Load"] = [](const std::string& p_path) { AmberTools::Global::ServiceLocator::Get<SceneSystem::SceneManager>().LoadAndPlayDelayed(p_path); };

	p_luaState["Resources"]["GetModel"] = [](const std::string& p_path) { return AmberTools::Global::ServiceLocator::Get<ResourceManagement::ModelManager>().GetResource(p_path); };
	p_luaState["Resources"]["GetShader"] = [](const std::string& p_path) { return AmberTools::Global::ServiceLocator::Get<ResourceManagement::ShaderManager>().GetResource(p_path); };
	p_luaState["Resources"]["GetTexture"] = [](const std::string& p_path) { return AmberTools::Global::ServiceLocator::Get<ResourceManagement::TextureManager>().GetResource(p_path); };
	p_luaState["Resources"]["GetMaterial"] = [](const std::string& p_path) { return AmberTools::Global::ServiceLocator::Get<ResourceManagement::MaterialManager>().GetResource(p_path); };
	p_luaState["Resources"]["GetSound"] = [](const std::string& p_path) { return AmberTools::Global::ServiceLocator::Get<ResourceManagement::SoundManager>().GetResource(p_path); };

	p_luaState["Math"]["RandomInt"] = [](int p_min, int p_max) { return AmberTools::Utils::Random::Generate(p_min, p_max); };
	p_luaState["Math"]["RandomFloat"] = [](float p_min, float p_max) { return AmberTools::Utils::Random::Generate(p_min, p_max); };
	p_luaState["Math"]["CheckPercentage"] = [](float p_percentage) { return AmberTools::Utils::Random::CheckPercentage(p_percentage); };
	p_luaState["Math"]["Lerp"] = [](float p_start, float p_end, float p_alpha) { return p_start + p_alpha * (p_end - p_start); };

	p_luaState.new_usertype<ECS::PhysicsWrapper::RaycastHit>("RaycastHit",
		"FirstResultObject", &ECS::PhysicsWrapper::RaycastHit::FirstResultObject,
		"ResultObjects", &ECS::PhysicsWrapper::RaycastHit::ResultObjects
		);

	p_luaState["Physics"]["Raycast"] = [](sol::this_state p_state, const glm::vec3& p_origin, const glm::vec3& p_direction, float p_distance)
	{
		sol::object result = sol::nil;

		ECS::PhysicsWrapper::RaycastHit hit;

		if (ECS::PhysicsWrapper::Raycast(p_origin, p_direction, p_distance, hit))
		{
			result = sol::object(p_state, sol::in_place, hit);
		}

		return result;
	};
}
