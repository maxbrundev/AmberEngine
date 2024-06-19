#pragma once

#include "AmberEngine/Tools/FileSystem/tinyxml2.h"

namespace AmberEngine::Core::API
{
	class ISerializable
	{
	public:

		virtual ~ISerializable() = default;
		
		virtual void OnSerialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node) = 0;
		virtual void OnDeserialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node) = 0;
	};
}
