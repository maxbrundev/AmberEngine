#include "Amberpch.h"

#include "AmberCore/Scripting/LuaMathsBinder.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

void AmberCore::Scripting::LuaMathsBinder::BindMaths(sol::state& p_luaState)
{
	p_luaState.new_usertype<glm::vec2>("Vector2",
		sol::constructors
		<
		glm::vec2(),
		glm::vec2(float, float)
		>(),

		sol::meta_function::addition, [](const glm::vec2& p_left, const glm::vec2& p_right) { return p_left + p_right; },
		sol::meta_function::subtraction, [](const glm::vec2& p_left, const glm::vec2& p_right) { return p_left - p_right; },
		sol::meta_function::unary_minus, [](const glm::vec2& p_target) { return -p_target; },
		sol::meta_function::multiplication, [](const glm::vec2& p_target, float p_scalar) { return p_target * p_scalar; },
		sol::meta_function::division, [](const glm::vec2& p_target, float p_scalar) { return p_target / p_scalar; },
		sol::meta_function::to_string, [](const glm::vec2& p_target) { return "(" + std::to_string(p_target.x) + "," + std::to_string(p_target.y) + ")"; },

		"x", &glm::vec2::x,
		"y", &glm::vec2::y,

		"One", []() { return glm::vec2(1.0f, 1.0f); },
		"Zero", []() { return glm::vec2(0.0f, 0.0f); },

		"Length", [](const glm::vec2& p_target) { return glm::length(p_target); },
		"Dot", [](const glm::vec2& p_left, const glm::vec2& p_right) { return glm::dot(p_left, p_right); },
		"Normalize", [](const glm::vec2& p_target) { const float length = glm::length(p_target); return length > 0.0f ? p_target / length : glm::vec2(0.0f); },
		"Lerp", [](const glm::vec2& p_start, const glm::vec2& p_end, float p_alpha) { return glm::mix(p_start, p_end, p_alpha); },
		"AngleBetween", [](const glm::vec2& p_from, const glm::vec2& p_to) { return std::acos(glm::dot(glm::normalize(p_from), glm::normalize(p_to))); }
		);

	p_luaState.new_usertype<glm::vec3>("Vector3",
		sol::constructors
		<
		glm::vec3(),
		glm::vec3(float, float, float)
		>(),

		sol::meta_function::addition, [](const glm::vec3& p_left, const glm::vec3& p_right) { return p_left + p_right; },
		sol::meta_function::subtraction, [](const glm::vec3& p_left, const glm::vec3& p_right) { return p_left - p_right; },
		sol::meta_function::unary_minus, [](const glm::vec3& p_target) { return -p_target; },
		sol::meta_function::multiplication, [](const glm::vec3& p_target, float p_scalar) { return p_target * p_scalar; },
		sol::meta_function::division, [](const glm::vec3& p_target, float p_scalar) { return p_target / p_scalar; },
		sol::meta_function::to_string, [](const glm::vec3& p_target) { return "(" + std::to_string(p_target.x) + "," + std::to_string(p_target.y) + "," + std::to_string(p_target.z) + ")"; },

		"x", &glm::vec3::x,
		"y", &glm::vec3::y,
		"z", &glm::vec3::z,

		"One", []() { return glm::vec3(1.0f, 1.0f, 1.0f); },
		"Zero", []() { return glm::vec3(0.0f, 0.0f, 0.0f); },
		"Forward", []() { return glm::vec3(0.0f, 0.0f, 1.0f); },
		"Up", []() { return glm::vec3(0.0f, 1.0f, 0.0f); },
		"Right", []() { return glm::vec3(1.0f, 0.0f, 0.0f); },
		"Backward", []() { return glm::vec3(0.0f, 0.0f, -1.0f); },
		"Down", []() { return glm::vec3(0.0f, -1.0f, 0.0f); },
		"Left", []() { return glm::vec3(-1.0f, 0.0f, 0.0f); },

		"Length", [](const glm::vec3& p_target) { return glm::length(p_target); },
		"Dot", [](const glm::vec3& p_left, const glm::vec3& p_right) { return glm::dot(p_left, p_right); },
		"Cross", [](const glm::vec3& p_left, const glm::vec3& p_right) { return glm::cross(p_left, p_right); },
		"Normalize", [](const glm::vec3& p_target) { const float length = glm::length(p_target); return length > 0.0f ? p_target / length : glm::vec3(0.0f); },
		"Lerp", [](const glm::vec3& p_start, const glm::vec3& p_end, float p_alpha) { return glm::mix(p_start, p_end, p_alpha); },
		"AngleBetween", [](const glm::vec3& p_from, const glm::vec3& p_to) { return std::acos(glm::dot(glm::normalize(p_from), glm::normalize(p_to))); },
		"Distance", [](const glm::vec3& p_left, const glm::vec3& p_right) { return glm::distance(p_left, p_right); }
		);

	p_luaState.new_usertype<glm::vec4>("Vector4",
		sol::constructors
		<
		glm::vec4(),
		glm::vec4(float, float, float, float)
		>(),

		sol::meta_function::addition, [](const glm::vec4& p_left, const glm::vec4& p_right) { return p_left + p_right; },
		sol::meta_function::subtraction, [](const glm::vec4& p_left, const glm::vec4& p_right) { return p_left - p_right; },
		sol::meta_function::unary_minus, [](const glm::vec4& p_target) { return -p_target; },
		sol::meta_function::multiplication, [](const glm::vec4& p_target, float p_scalar) { return p_target * p_scalar; },
		sol::meta_function::division, [](const glm::vec4& p_target, float p_scalar) { return p_target / p_scalar; },
		sol::meta_function::to_string, [](const glm::vec4& p_target) { return "(" + std::to_string(p_target.x) + "," + std::to_string(p_target.y) + "," + std::to_string(p_target.z) + "," + std::to_string(p_target.w) + ")"; },

		"x", &glm::vec4::x,
		"y", &glm::vec4::y,
		"z", &glm::vec4::z,
		"w", &glm::vec4::w,

		"One", []() { return glm::vec4(1.0f, 1.0f, 1.0f, 1.0f); },
		"Zero", []() { return glm::vec4(0.0f, 0.0f, 0.0f, 0.0f); },

		"Length", [](const glm::vec4& p_target) { return glm::length(p_target); },
		"Dot", [](const glm::vec4& p_left, const glm::vec4& p_right) { return glm::dot(p_left, p_right); },
		"Normalize", [](const glm::vec4& p_target) { const float length = glm::length(p_target); return length > 0.0f ? p_target / length : glm::vec4(0.0f); },
		"Lerp", [](const glm::vec4& p_start, const glm::vec4& p_end, float p_alpha) { return glm::mix(p_start, p_end, p_alpha); }
		);

	p_luaState.new_usertype<glm::mat3>("Matrix3",
		sol::constructors
		<
		glm::mat3(),
		glm::mat3(float)
		>(),

		sol::meta_function::addition, [](const glm::mat3& p_left, const glm::mat3& p_right) { return p_left + p_right; },
		sol::meta_function::subtraction, [](const glm::mat3& p_left, const glm::mat3& p_right) { return p_left - p_right; },
		sol::meta_function::multiplication, sol::overload
		(
			[](const glm::mat3& p_target, float p_scalar) { return p_target * p_scalar; },
			[](const glm::mat3& p_target, const glm::vec3& p_vector) { return p_target * p_vector; },
			[](const glm::mat3& p_left, const glm::mat3& p_right) { return p_left * p_right; }
		),
		sol::meta_function::division, [](const glm::mat3& p_target, float p_scalar) { return p_target / p_scalar; },
		sol::meta_function::to_string, [](const glm::mat3& p_target) { return "Can't show matrix as string for now"; },

		"Identity", []() { return glm::mat3(1.0f); },

		"IsIdentity", [](const glm::mat3& p_target) { return p_target == glm::mat3(1.0f); },
		"Determinant", [](const glm::mat3& p_target) { return glm::determinant(p_target); },
		"Transpose", [](const glm::mat3& p_target) { return glm::transpose(p_target); },
		"Inverse", [](const glm::mat3& p_target) { return glm::inverse(p_target); },
		"Get", [](glm::mat3& p_target, int p_row, int p_column) { return p_target[p_column][p_row]; },
		"Set", [](glm::mat3& p_target, int p_row, int p_column, float p_value) { p_target[p_column][p_row] = p_value; }
	);

	p_luaState.new_usertype<glm::mat4>("Matrix4",
		sol::constructors
		<
		glm::mat4(),
		glm::mat4(float)
		>(),

		sol::meta_function::addition, [](const glm::mat4& p_left, const glm::mat4& p_right) { return p_left + p_right; },
		sol::meta_function::subtraction, [](const glm::mat4& p_left, const glm::mat4& p_right) { return p_left - p_right; },
		sol::meta_function::multiplication, sol::overload
		(
			[](const glm::mat4& p_target, float p_scalar) { return p_target * p_scalar; },
			[](const glm::mat4& p_target, const glm::vec4& p_vector) { return p_target * p_vector; },
			[](const glm::mat4& p_left, const glm::mat4& p_right) { return p_left * p_right; }
		),
		sol::meta_function::division, [](const glm::mat4& p_target, float p_scalar) { return p_target / p_scalar; },
		sol::meta_function::to_string, [](const glm::mat4& p_target) { return "Can't show matrix as string for now"; },

		"Identity", []() { return glm::mat4(1.0f); },

		"IsIdentity", [](const glm::mat4& p_target) { return p_target == glm::mat4(1.0f); },
		"Determinant", [](const glm::mat4& p_target) { return glm::determinant(p_target); },
		"Transpose", [](const glm::mat4& p_target) { return glm::transpose(p_target); },
		"Inverse", [](const glm::mat4& p_target) { return glm::inverse(p_target); },
		"Translation", [](const glm::vec3& p_translation) { return glm::translate(glm::mat4(1.0f), p_translation); },
		"Translate", [](const glm::mat4& p_target, const glm::vec3& p_translation) { return glm::translate(p_target, p_translation); },
		"RotationOnAxisX", [](float p_angle) { return glm::rotate(glm::mat4(1.0f), p_angle, glm::vec3(1.0f, 0.0f, 0.0f)); },
		"RotateOnAxisX", [](const glm::mat4& p_target, float p_angle) { return glm::rotate(p_target, p_angle, glm::vec3(1.0f, 0.0f, 0.0f)); },
		"RotationOnAxisY", [](float p_angle) { return glm::rotate(glm::mat4(1.0f), p_angle, glm::vec3(0.0f, 1.0f, 0.0f)); },
		"RotateOnAxisY", [](const glm::mat4& p_target, float p_angle) { return glm::rotate(p_target, p_angle, glm::vec3(0.0f, 1.0f, 0.0f)); },
		"RotationOnAxisZ", [](float p_angle) { return glm::rotate(glm::mat4(1.0f), p_angle, glm::vec3(0.0f, 0.0f, 1.0f)); },
		"RotateOnAxisZ", [](const glm::mat4& p_target, float p_angle) { return glm::rotate(p_target, p_angle, glm::vec3(0.0f, 0.0f, 1.0f)); },
		"Scaling", [](const glm::vec3& p_scale) { return glm::scale(glm::mat4(1.0f), p_scale); },
		"Scale", [](const glm::mat4& p_target, const glm::vec3& p_scale) { return glm::scale(p_target, p_scale); },
		"CreatePerspective", [](float p_fov, float p_aspectRatio, float p_zNear, float p_zFar) { return glm::perspective(glm::radians(p_fov), p_aspectRatio, p_zNear, p_zFar); },
		"CreateView", [](const glm::vec3& p_eye, const glm::vec3& p_center, const glm::vec3& p_up) { return glm::lookAt(p_eye, p_center, p_up); },
		"Get", [](glm::mat4& p_target, int p_row, int p_column) { return p_target[p_column][p_row]; },
		"Set", [](glm::mat4& p_target, int p_row, int p_column, float p_value) { p_target[p_column][p_row] = p_value; }
	);

	p_luaState.new_usertype<glm::quat>("Quaternion",
		sol::call_constructor, sol::factories
		(
			[]() { return glm::quat(1.0f, 0.0f, 0.0f, 0.0f); },
			[](float p_x, float p_y, float p_z, float p_w) { return glm::quat(p_w, p_x, p_y, p_z); },
			[](const glm::vec3& p_eulerAngles) { return glm::quat(glm::radians(p_eulerAngles)); }
		),
		"new", sol::factories
		(
			[]() { return glm::quat(1.0f, 0.0f, 0.0f, 0.0f); },
			[](float p_x, float p_y, float p_z, float p_w) { return glm::quat(p_w, p_x, p_y, p_z); },
			[](const glm::vec3& p_eulerAngles) { return glm::quat(glm::radians(p_eulerAngles)); }
		),

		sol::meta_function::addition, [](const glm::quat& p_left, const glm::quat& p_right) { return p_left + p_right; },
		sol::meta_function::multiplication, sol::overload
		(
			[](const glm::quat& p_target, float p_scalar) { return p_target * p_scalar; },
			[](const glm::quat& p_left, const glm::quat& p_right) { return p_left * p_right; },
			[](const glm::quat& p_target, const glm::vec3& p_vector) { return p_target * p_vector; }
		),
		sol::meta_function::to_string, [](const glm::quat& p_target) { return "(" + std::to_string(p_target.x) + "," + std::to_string(p_target.y) + "," + std::to_string(p_target.z) + "," + std::to_string(p_target.w) + ")"; },

		"IsIdentity", [](const glm::quat& p_target) { return p_target == glm::quat(1.0f, 0.0f, 0.0f, 0.0f); },
		"Dot", [](const glm::quat& p_left, const glm::quat& p_right) { return glm::dot(p_left, p_right); },
		"Normalize", [](const glm::quat& p_target) { return glm::normalize(p_target); },
		"Length", [](const glm::quat& p_target) { return glm::length(p_target); },
		"Inverse", [](const glm::quat& p_target) { return glm::inverse(p_target); },
		"Conjugate", [](const glm::quat& p_target) { return glm::conjugate(p_target); },
		"Lerp", [](const glm::quat& p_start, const glm::quat& p_end, float p_alpha) { return glm::lerp(p_start, p_end, p_alpha); },
		"Slerp", [](const glm::quat& p_start, const glm::quat& p_end, float p_alpha) { return glm::slerp(p_start, p_end, p_alpha); },
		"RotatePoint", [](const glm::vec3& p_point, const glm::quat& p_quaternion) { return p_quaternion * p_point; },
		"EulerAngles", [](const glm::quat& p_target) { return glm::degrees(glm::eulerAngles(p_target)); },
		"ToMatrix3", [](const glm::quat& p_target) { return glm::mat3_cast(p_target); },
		"ToMatrix4", [](const glm::quat& p_target) { return glm::mat4_cast(p_target); },

		"x", &glm::quat::x,
		"y", &glm::quat::y,
		"z", &glm::quat::z,
		"w", &glm::quat::w
		);
}
