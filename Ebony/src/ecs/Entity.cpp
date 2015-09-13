#include "ecs/ecs.h"

namespace ebony { namespace ecs {

	Entity::Entity() : _id(0), _version(0)
	{}

	Entity::Entity(EntityManager *manager, EntityId id, EntityVersion version) :
		_id(id), _version(version), _manager(manager)
	{}

	Entity::Entity(const Entity &entity) :
		_id(entity._id), _version(entity._version), _manager(entity._manager)
	{}

	Entity &Entity::operator=(const Entity &entity)
	{
		_id = entity._id;
		_version = entity._version;
		_manager = entity._manager;
	}

	bool Entity::operator==(const Entity &entity) const
	{
		return _version == entity._version && _id == entity._id && _manager == entity._manager;
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
		if (!_manager) {
			return false;
		}

		return _manager->isEntityValid(*this);
	}

	void Entity::destroy()
	{
		if (_manager) {
			_manager->destroy(*this);
			_manager = nullptr;
		}
	}

}
}
