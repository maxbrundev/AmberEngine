#include "Amberpch.h"

#include "AmberEngine/Data/Color.h"

const AmberEngine::Data::Color AmberEngine::Data::Color::Red            = { 1.f, 0.f, 0.f };
const AmberEngine::Data::Color AmberEngine::Data::Color::Color::Green   = { 0.f, 1.f, 0.f };
const AmberEngine::Data::Color AmberEngine::Data::Color::Color::Blue    = { 0.f, 0.f, 1.f };
const AmberEngine::Data::Color AmberEngine::Data::Color::Color::White   = { 1.f, 1.f, 1.f };
const AmberEngine::Data::Color AmberEngine::Data::Color::Color::Black   = { 0.f, 0.f, 0.f };
const AmberEngine::Data::Color AmberEngine::Data::Color::Color::Grey    = { 0.5f, 0.5f, 0.5f };
const AmberEngine::Data::Color AmberEngine::Data::Color::Color::Yellow  = { 1.f, 1.f, 0.f };
const AmberEngine::Data::Color AmberEngine::Data::Color::Color::Cyan    = { 0.f, 1.f, 1.f };
const AmberEngine::Data::Color AmberEngine::Data::Color::Color::Magenta = { 1.f, 0.f, 1.f };

const AmberEngine::Data::Color AmberEngine::Data::Color::Color::EditorYellow = { 1.f, 0.8f, 0.01f };

AmberEngine::Data::Color::Color(float p_r, float p_g, float p_b, float p_a) : r(p_r), g(p_g), b(p_b), a(p_a)
{
}

AmberEngine::Data::Color::Color(uint32_t p_packedData) :
Color
(
	static_cast<float>(p_packedData >> 24 & 0xff) * 0.0039215686274509803921568627451,
	static_cast<float>(p_packedData >> 16 & 0xff) * 0.0039215686274509803921568627451,
	static_cast<float>(p_packedData >> 8 & 0xff)  * 0.0039215686274509803921568627451
)
{
}

AmberEngine::Data::Color::Color(const glm::vec3& p_standardColor) :
	Color
	(
		p_standardColor.x / 255.0f,
		p_standardColor.y / 255.0f,
		p_standardColor.z / 255.0f
	)
{
}

AmberEngine::Data::Color::Color(const glm::vec4& p_standardColor) :
	Color
	(
		p_standardColor.x / 255.0f,
		p_standardColor.y / 255.0f,
		p_standardColor.z / 255.0f,
		p_standardColor.w / 255.0f

	)
{
}

uint32_t AmberEngine::Data::Color::Pack() const
{
	return (static_cast<uint8_t>(r * 255.0f) << 24) | (static_cast<uint8_t>(g * 255.0f) << 16) | (static_cast<uint8_t>(b * 255.0f) << 8) | static_cast<uint8_t>(a);
}

std::tuple<float, float, float, float> AmberEngine::Data::Color::GetStandardized()
{
	return std::make_tuple(r * 255.0f, g * 255.0f, b * 255.0f, a);
}

glm::vec3 AmberEngine::Data::Color::GetVec3() const
{
	return {r, g, b};
}

glm::vec4 AmberEngine::Data::Color::GetVec4() const
{
	return { r, g, b, a };
}

AmberEngine::Data::Color& AmberEngine::Data::Color::operator+=(const Color& p_color)
{
	r += p_color.r;
	g += p_color.g;
	b += p_color.b;
	a += p_color.a;

	return *this;
}

bool AmberEngine::Data::Color::operator==(const Color& p_other) const
{
	return this->r == p_other.r && this->g == p_other.g && this->b == p_other.b && this->a == p_other.a;
}

bool AmberEngine::Data::Color::operator!=(const Color& p_other) const
{
	return !operator==(p_other);
}

AmberEngine::Data::Color AmberEngine::Data::Color::Mix(const Color& p_color1, const Color& p_color2, float p_alpha)
{
	return Color(glm::mix(p_color1.GetVec3(), p_color2.GetVec3(), p_alpha));
}
