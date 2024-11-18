#pragma once

#include <bullet/btBulletCollisionCommon.h>

namespace AmberEngine::Physics::Settings
{
	struct BodySettings
	{
		btVector3 linearVelocity	= { 0.0f, 0.0f, 0.0f };
		btVector3 angularVelocity	= { 0.0f, 0.0f, 0.0f };
		btVector3 linearFactor		= { 1, 1, 1 };
		btVector3 angularFactor		= { 1, 1, 1 };
		float restitution			= 0.0f;
		float friction				= 0.5f;
		bool isTrigger				= false;
		bool isKinematic			= false;
	};
}