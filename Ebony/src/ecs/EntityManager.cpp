#include "ecs/ecs.h"

#include <limits>
#include <stdexcept>

using namespace std;

namespace ebony { namespace ecs {

	shared_ptr<EntityManager> EntityManager::makeInstance()
	{
		return shared_ptr<EntityManager>(new EntityManager());
	}

	Entity EntityManager::create()
	{
		EntityId id;
		EntityVersion version;

		if (_freeList.empty()) {
			if (_nextEntity > numeric_limits<EntityId>::max()) {
				throw runtime_error("Entity id out of bounds");
			}

			growCapacity();
			id = _nextEntity++;
			version = _entityVersion[id] = 1;
		} else {
			id = _freeList.back();
			_freeList.pop_back();
			version = _entityVersion[id];
		}

		return Entity(shared_from_this(), id, version);
	}

	void EntityManager::destroy(Entity &entity)
	{
		if (!entity || entity._manager.lock() != shared_from_this()) {
			return;
		}

		++_entityVersion[entity._id];
		_componentMasks[entity._id].reset();
		_freeList.push_back(entity._id);
		entity._manager.reset();
	}

	bool EntityManager::isEntityValid(const Entity &entity) const
	{
		return entity._id < _nextEntity && entity._version == _entityVersion[entity._id];
	}

	void EntityManager::growCapacity()
	{
		size_t newSize;

		if (_entityVersion.size() > 0) {
			newSize = _entityVersion.size();

			while (newSize <= _nextEntity) {
				newSize *= 2;
			}
		} else {
			newSize = 256;
		}

		_entityVersion.resize(newSize, 0);
		_componentMasks.resize(newSize, ComponentMask());

		for (vector<void *> components : _components) {
			components.resize(newSize, nullptr);
		}
	}

	void EntityManager::accomodateComponents()
	{
		size_t oldSize = _componentPools.size();

		_componentPools.resize(_nbComponentTypes, nullptr);
		_components.resize(_nbComponentTypes);

		for (unsigned int i = oldSize; i < _nbComponentTypes; ++i) {
			_components[i].resize(_componentMasks.size(), nullptr);
		}
	}

	EntityManager::Iterator::Iterator(const EntityManager::Iterator &it) :
		_mask(it._mask), _current(it._current), _manager(it._manager)
	{
		goToFirstValid();
	}

	EntityManager::Iterator &EntityManager::Iterator::operator=(const EntityManager::Iterator &it)
	{
		_mask = it._mask;
		_manager = it._manager;
		_current = it._current;
		goToFirstValid();
	}

	EntityManager::Iterator::Iterator(weak_ptr<EntityManager> manager,
									  const ComponentMask &mask,
									  EntityId current) :
		_mask(mask), _manager(manager), _current(current)
	{
		goToFirstValid();
	}

	bool EntityManager::Iterator::operator==(const EntityManager::Iterator &it)
	{
		return _mask == it._mask
			&& _current == it._current
			&& _manager.lock() == it._manager.lock();
	}

	bool EntityManager::Iterator::operator!=(const EntityManager::Iterator &it)
	{
		return _mask != it._mask
			|| _current != it._current
			|| _manager.lock() != it._manager.lock();
	}

	Entity EntityManager::Iterator::operator*() const
	{
		return Entity(_manager, _current, _manager.lock()->_entityVersion[_current]);
	}

	EntityManager::Iterator &EntityManager::Iterator::operator++()
	{
		if (_current < _manager.lock()->_nextEntity) {
			_current++;
			goToFirstValid();
		}

		return *this;
	}

	void EntityManager::Iterator::goToFirstValid()
	{
		shared_ptr<EntityManager> manager = _manager.lock();

		while (_current < manager->_nextEntity) {
			if ((manager->_componentMasks[_current] & _mask) == _mask) {
				break;
			}

			_current++;
		}
	}

	EntityManager::EntityView::EntityView(shared_ptr<EntityManager> manager,
										  const ComponentMask &mask) :
		_mask(mask), _manager(manager)
	{}

	EntityManager::Iterator EntityManager::EntityView::begin()
	{
		return EntityManager::Iterator(_manager, _mask);
	}

	EntityManager::Iterator EntityManager::EntityView::end()
	{
		if (!_manager.lock()) {
			throw runtime_error("Reference to manager expired");
		}

		return EntityManager::Iterator(_manager, _mask, _manager.lock()->_nextEntity);
	}

	const EntityManager::Iterator EntityManager::EntityView::begin() const
	{
		return EntityManager::Iterator(_manager, _mask);
	}

	const EntityManager::Iterator EntityManager::EntityView::end() const
	{
		if (!_manager.lock()) {
			throw runtime_error("Reference to manager expired");
		}

		return EntityManager::Iterator(_manager, _mask, _manager.lock()->_nextEntity);
	}

}
}
