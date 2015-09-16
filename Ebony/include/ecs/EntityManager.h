#ifndef EBONY_ECS_ENTITY_MANAGER_H_
#define EBONY_ECS_ENTITY_MANAGER_H_

#include <vector>
#include <memory>
#include <cassert>

#include "constants.h"
#include "Entity.h"
#include "Component.h"
#include "utils/Pool.h"

namespace ebony { namespace ecs {

	class EntityManager : public std::enable_shared_from_this<EntityManager> {
	public:
		EntityManager(const EntityManager &manager) = delete;
		EntityManager &operator=(const EntityManager &manager) = delete;

		static std::shared_ptr<EntityManager> makeInstance();

		Entity create();

	private:
		friend Entity;

		EntityManager() = default;

		void destroy(Entity &entity);
		void growCapacity();
		void accomodateComponents();
		bool isEntityValid(const Entity &entity) const;

		template<typename T>
		inline unsigned int getComponentId()
		{
			static unsigned int id = _nbComponentTypes++;

			assert(id < MAX_COMPONENTS);

			return id;
		}

		template<typename T>
		Pool<T> *createPool()
		{
			Pool<T> *pool = new Pool<T>();

			_componentPools[getComponentId<T>()] = reinterpret_cast<IPool *>(pool);

			return pool;
		}

		template<typename T>
		inline bool hasComponents_(const Entity &entity)
		{
			return _componentMasks[entity._id][getComponentId<T>()];
		}

		template<typename T1, typename T2, typename ... Ts>
		inline bool hasComponents_(const Entity &entity)
		{
			return hasComponents_<T1>(entity) && hasComponents_<T2, Ts ...>(entity);
		}

		template<typename T>
		inline bool hasComponents(const Entity &entity)
		{
			if (!isEntityValid(entity)) {
				return false;
			}

			return hasComponents_<T>(entity);
		}

		template<typename T1, typename T2, typename ... Ts>
		inline bool hasComponents(const Entity &entity)
		{
			if (!isEntityValid(entity)) {
				return false;
			}

			return hasComponents_<T1, T2, Ts ...>(entity);
		}

		template<typename T>
		Component<T> addComponent(const Entity &entity)
		{
			unsigned int id = getComponentId<T>();

			if (!isEntityValid(entity)) {
				return Component<T>();
			}

			if (hasComponents<T>(entity)) {
				return Component<T>(entity, static_cast<T *>(_components[id][entity._id]));
			}

			if (_componentPools.size() <= id) {
				accomodateComponents();
			}

			Pool<T> *pool = static_cast<Pool<T> *>(_componentPools[id]);

			if (!pool) {
				pool = createPool<T>();
			}

			T *componentPtr = static_cast<T *>(pool->allocate());

			_componentMasks[entity._id][id] = true;
			_components[id][entity._id] = componentPtr;

			return Component<T>(entity, componentPtr);
		}

		template<typename T>
		Component<T> getComponent(const Entity &entity)
		{
			unsigned int id = getComponentId<T>();

			if (!isEntityValid(entity)) {
				return Component<T>();
			}

			if (_componentPools.size() <= id) {
				accomodateComponents();
			}

			if (!hasComponents<T>(entity)) {
				return Component<T>();
			}

			return Component<T>(entity, static_cast<T *>(_components[id][entity._id]));
		}

		template<typename T>
		void removeComponent(const Entity &entity)
		{
			unsigned int id = getComponentId<T>();

			if (!isEntityValid(entity)) {
				return;
			}

			if (_componentPools.size() <= id) {
				accomodateComponents();
			}

			if (!hasComponents<T>(entity)) {
				return;
			}

			T *componentPtr = static_cast<T *>(_components[id][entity._id]);
			Pool<T> *pool = static_cast<Pool<T> *>(_componentPools[id]);

			pool->free(componentPtr);
			_componentMasks[entity._id][id] = false;
			_components[id][entity._id] = nullptr;
		}

		unsigned int _nbComponentTypes = 0;
		EntityId _nextEntity = 0;
		std::vector<EntityId> _freeList;
		std::vector<EntityVersion> _entityVersion;
		std::vector<ComponentMask> _componentMasks;
		std::vector<IPool *> _componentPools;
		std::vector<std::vector<void *>> _components;
	};

}
}

#endif
