#include "stdafx.h"
#include "ecs/Entity.h"

#include "ecs/EntityManager.h"

namespace ebony { namespace ecs {

Entity::Entity(std::shared_ptr<EntityManager> manager, EntityId id, EntityVersion version) :
	_id(id), _version(version), _manager(manager)
{}

Entity::Entity(std::weak_ptr<EntityManager> manager, EntityId id, EntityVersion version) :
	_id(id), _version(version), _manager(manager)
{}

bool Entity::operator==(const Entity &entity) const
{
	std::shared_ptr<EntityManager> manager1 = _manager.lock();
	std::shared_ptr<EntityManager> manager2 = entity._manager.lock();

	if (!manager1 || !manager2) {
		return false;
	}

	return _version == entity._version && _id == entity._id && manager1 == manager2;
}

bool Entity::operator!=(const Entity &entity) const
{
	return !(*this == entity);
}

Entity::operator bool() const
{
	return isValid();
}

bool Entity::isValid() const
{
	std::shared_ptr<EntityManager> manager = _manager.lock();

	if (!manager) {
		return false;
	}

	return manager->isEntityValid(*this);
}

void Entity::destroy()
{
	std::shared_ptr<EntityManager> manager = _manager.lock();

	if (manager) {
		manager->destroy(*this);
		_manager.reset();
	}
}

void Entity::removeAllComponents()
{
	std::shared_ptr<EntityManager> manager = _manager.lock();

	if (manager) {
		manager->removeAllComponents(*this);
	}
}

}}
