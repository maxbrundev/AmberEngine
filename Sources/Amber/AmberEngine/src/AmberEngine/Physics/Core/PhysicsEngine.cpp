#include "Amberpch.h"

#include "AmberEngine/Physics/Core/PhysicsEngine.h"
#include "AmberEngine/Physics/Entities/PhysicalObject.h"
#include "AmberEngine/Physics/Tools/Conversion.h"

#include "AmberEngine/Debug/Logger.h"

std::map<std::pair<AmberEngine::Physics::Entities::PhysicalObject*, AmberEngine::Physics::Entities::PhysicalObject*>, bool> AmberEngine::Physics::Core::PhysicsEngine::m_collisionEvents;

AmberEngine::Physics::Core::PhysicsEngine::PhysicsEngine(const Settings::PhysicsSettings & p_settings)
{
	m_collisionConfig = std::make_unique<btDefaultCollisionConfiguration>();
	m_dispatcher      = std::make_unique<btCollisionDispatcher>(m_collisionConfig.get());
	m_broadphase      = std::make_unique<btDbvtBroadphase>();
	m_solver          = std::make_unique<btSequentialImpulseConstraintSolver>();
	m_world           = std::make_unique<btDiscreteDynamicsWorld>(m_dispatcher.get(), m_broadphase.get(), m_solver.get(), m_collisionConfig.get());

	m_world->setGravity(Tools::Conversion::ToBtVector3(p_settings.gravity));

	ListenToPhysicalObjects();
	SetCollisionCallback();
}

void AmberEngine::Physics::Core::PhysicsEngine::PreUpdate()
{
	std::for_each(m_physicalObjects.begin(), m_physicalObjects.end(), std::mem_fn(&Entities::PhysicalObject::UpdateBtTransform));

	ResetCollisionEvents();
}

void AmberEngine::Physics::Core::PhysicsEngine::PostUpdate()
{
	std::for_each(m_physicalObjects.begin(), m_physicalObjects.end(), std::mem_fn(&Entities::PhysicalObject::UpdateFTransform));

	CheckCollisionStopEvents();
}

bool AmberEngine::Physics::Core::PhysicsEngine::Update(float p_deltaTime)
{
	PreUpdate();

	if (m_world->stepSimulation(p_deltaTime, 10))
	{
		PostUpdate();

		return true;
	}

	return false;
}

std::optional<AmberEngine::Physics::Entities::RaycastHit> AmberEngine::Physics::Core::PhysicsEngine::Raycast(glm::vec3 p_origin, glm::vec3 p_direction, float p_distance)
{
	if (p_direction == glm::vec3(0.0f))
		return {};

	btVector3 origin = Tools::Conversion::ToBtVector3(p_origin);
	btVector3 target = Tools::Conversion::ToBtVector3(p_origin + p_direction * p_distance);

	// Try to get First Hit
	btCollisionWorld::ClosestRayResultCallback ClosestRayCallback(origin, target);
	m_world->rayTest(origin, target, ClosestRayCallback);

	if (ClosestRayCallback.hasHit())
	{
		Entities::RaycastHit resultHit;

		// Get First Hit
		resultHit.FirstResultObject = reinterpret_cast<AmberEngine::Physics::Entities::PhysicalObject*>(ClosestRayCallback.m_collisionObject->getUserPointer());

		// Try to get all Hit
		btCollisionWorld::AllHitsRayResultCallback rayCallback(origin, target);
		m_world->rayTest(origin, target, rayCallback);

		// Get all Hit
		for (int i = 0; i < rayCallback.m_collisionObjects.size(); i++)
			resultHit.ResultObjects.push_back(reinterpret_cast<AmberEngine::Physics::Entities::PhysicalObject*>(rayCallback.m_collisionObjects[i]->getUserPointer()));

		return resultHit;
	}
	else
		return {};
}

void AmberEngine::Physics::Core::PhysicsEngine::SetGravity(const glm::vec3 & p_gravity)
{
	m_world->setGravity(Tools::Conversion::ToBtVector3(p_gravity));
}

glm::vec3 AmberEngine::Physics::Core::PhysicsEngine::GetGravity() const
{
	return Tools::Conversion::ToOvVector3(m_world->getGravity());
}

void AmberEngine::Physics::Core::PhysicsEngine::ListenToPhysicalObjects()
{
	Entities::PhysicalObject::CreatedEvent   += std::bind(static_cast<void(PhysicsEngine::*)(Entities::PhysicalObject&)>(&PhysicsEngine::Consider), this, std::placeholders::_1);
	Entities::PhysicalObject::DestroyedEvent += std::bind(static_cast<void(PhysicsEngine::*)(Entities::PhysicalObject&)>(&PhysicsEngine::Unconsider), this, std::placeholders::_1);

	Entities::PhysicalObject::ConsiderEvent   += std::bind(static_cast<void(PhysicsEngine::*)(btRigidBody&)>(&PhysicsEngine::Consider), this, std::placeholders::_1);
	Entities::PhysicalObject::UnconsiderEvent += std::bind(static_cast<void(PhysicsEngine::*)(btRigidBody&)>(&PhysicsEngine::Unconsider), this, std::placeholders::_1);
}

void AmberEngine::Physics::Core::PhysicsEngine::Consider(Entities::PhysicalObject& p_toConsider)
{
	m_physicalObjects.push_back(std::ref(p_toConsider));
}

void AmberEngine::Physics::Core::PhysicsEngine::Unconsider(Entities::PhysicalObject& p_toUnconsider)
{
	{
		auto found = std::find_if(m_physicalObjects.begin(), m_physicalObjects.end(), [&p_toUnconsider](std::reference_wrapper<Entities::PhysicalObject> element)
		{
			return std::addressof(p_toUnconsider) == std::addressof(element.get());
		});

		if (found != m_physicalObjects.end())
			m_physicalObjects.erase(found);
	}

	{
		decltype(m_collisionEvents)::iterator iter = m_collisionEvents.begin();
		decltype(m_collisionEvents)::iterator endIter = m_collisionEvents.end();

		for (; iter != endIter; )
		{
			if (iter->first.first == std::addressof(p_toUnconsider) || iter->first.second == std::addressof(p_toUnconsider))
			{
				m_collisionEvents.erase(iter++);
			}
			else 
			{
				++iter;
			}
		}
	}
}

void AmberEngine::Physics::Core::PhysicsEngine::Consider(btRigidBody& p_toConsider)
{
	m_world->addRigidBody(&p_toConsider);
}

void AmberEngine::Physics::Core::PhysicsEngine::Unconsider(btRigidBody& p_toUnconsider)
{
	m_world->removeRigidBody(&p_toUnconsider);
}

void AmberEngine::Physics::Core::PhysicsEngine::ResetCollisionEvents()
{
	for (auto& element : m_collisionEvents)
	{
		element.second = false;
	}
}

void AmberEngine::Physics::Core::PhysicsEngine::CheckCollisionStopEvents()
{
	for (auto it = m_collisionEvents.begin(); it != m_collisionEvents.end();)
	{
		auto objects = it->first;

		if (!it->second)
		{
			if (!objects.first->IsTrigger() && !objects.second->IsTrigger())
			{
				objects.first->CollisionStopEvent.Invoke(*objects.second);
				objects.second->CollisionStopEvent.Invoke(*objects.first);
			}
			else
			{
				if (objects.first->IsTrigger())
					objects.first->TriggerStopEvent.Invoke(*objects.second);
				else
					objects.second->TriggerStopEvent.Invoke(*objects.first);
			}

			it = m_collisionEvents.erase(it);
		}
		else
		{
			++it;
		}
	}
}

bool AmberEngine::Physics::Core::PhysicsEngine::CollisionCallback(btManifoldPoint& cp, const btCollisionObjectWrapper* obj1, int id1, int index1, const btCollisionObjectWrapper* obj2, int id2, int index2)
{
	auto object1 = reinterpret_cast<Entities::PhysicalObject*>(obj1->getCollisionObject()->getUserPointer());
	auto object2 = reinterpret_cast<Entities::PhysicalObject*>(obj2->getCollisionObject()->getUserPointer());

	if (object1 && object2)
	{
		/* If the objects are not all trigger, enter */
		if (!object1->IsTrigger() || !object2->IsTrigger())
		{
			if (m_collisionEvents.find({ object1 , object2 }) == m_collisionEvents.end())
			{
				/* If object is trigger, invoke Trigger event,
				 * else : is the other object trigger ? yes -> do nothing, no -> invoke Collision event
				 */

				 // Object 1 (Start event)
				if (object1->IsTrigger())
				{
					object1->TriggerStartEvent.Invoke(*object2);
				}
				else
				{
					if (!object2->IsTrigger())
						object1->CollisionStartEvent.Invoke(*object2);
				}
				// Object 2 (Start event)
				if (object2->IsTrigger())
				{
					object2->TriggerStartEvent.Invoke(*object1);
				}
				else
				{
					if (!object1->IsTrigger())
						object2->CollisionStartEvent.Invoke(*object1);
				}

				// Object 1 (Stay event)
				if (object1->IsTrigger())
				{
					object1->TriggerStayEvent.Invoke(*object2);
				}
				else
				{
					if (!object2->IsTrigger())
						object1->CollisionStayEvent.Invoke(*object2);
				}
				// Object 2 (Stay event)
				if (object2->IsTrigger())
				{
					object2->TriggerStayEvent.Invoke(*object1);
				}
				else
				{
					if (!object1->IsTrigger())
						object2->CollisionStayEvent.Invoke(*object1);
				}

				m_collisionEvents[{ object1, object2 }] = true;
			}
			else
			{
				if (!m_collisionEvents[{ object1, object2 }])
				{
					// Object 1 (Stay event)
					if (object1->IsTrigger())
					{
						object1->TriggerStayEvent.Invoke(*object2);
					}
					else
					{
						if (!object2->IsTrigger())
							object1->CollisionStayEvent.Invoke(*object2);
					}
					// Object 2 (Stay event)
					if (object2->IsTrigger())
					{
						object2->TriggerStayEvent.Invoke(*object1);
					}
					else
					{
						if (!object1->IsTrigger())
							object2->CollisionStayEvent.Invoke(*object1);
					}

					m_collisionEvents[{ object1, object2 }] = true;
				}
			}
		}
	}

	return false;
}

void AmberEngine::Physics::Core::PhysicsEngine::SetCollisionCallback()
{
	gContactAddedCallback = &PhysicsEngine::CollisionCallback;
}
