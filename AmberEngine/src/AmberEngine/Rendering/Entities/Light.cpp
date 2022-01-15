#include "Amberpch.h"

#include "AmberEngine/Rendering/Entities/Light.h"

AmberEngine::Rendering::Entities::Light::Light(Maths::Transform& p_tranform, ELightType p_type) : type(static_cast<float>(p_type)), m_transform(p_tranform)
{
}
