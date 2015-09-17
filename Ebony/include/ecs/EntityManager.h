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
			const static unsigned int id = _nbComponentTypes++;

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
		inline void createComponentMask(ComponentMask &mask)
		{
			mask.set(getComponentId<T>());
		}

		template<typename T1, typename T2, typename ... Ts>
		inline void createComponentMask(ComponentMask &mask)
		{
			mask.set(getComponentId<T1>());
			createComponentMask<T2, Ts ...>(mask);
		}

		template<typename ... Ts>
		inline ComponentMask createComponentMask()
		{
			static ComponentMask mask;

			createComponentMask<Ts ...>(mask);

			return mask;
		}

		template<typename ... Ts>
		inline bool hasComponents(const Entity &entity)
		{
			if (!isEntityValid(entity)) {
				return false;
			}

			static const ComponentMask compMask = createComponentMask<Ts ...>();

			return (_componentMasks[entity._id] & compMask) == compMask;
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

			_componentMasks[entity._id].set(id);
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
			_componentMasks[entity._id].reset(id);
			_components[id][entity._id] = nullptr;
		}

		unsigned int _nbComponentTypes = 0;
		EntityId _nextEntity = 0;
		std::vector<EntityId> _freeList;
		std::vector<EntityVersion> _entityVersion;
		std::vector<ComponentMask> _componentMasks;
		std::vector<IPool *> _componentPools;
		std::vector<std::vector<void *>> _components;

	public:
		class Iterator {
		public:
			Iterator(const Iterator &it);

			Iterator &operator=(const Iterator &it);

			bool operator==(const Iterator &it);
			bool operator!=(const Iterator &it);

			Entity operator*() const;
			Iterator &operator++();

			EntityId _current;

		private:
			friend EntityManager;

			Iterator() = default;
			Iterator(std::weak_ptr<EntityManager> manager,
					 const ComponentMask &mask,
					 EntityId current = 0);

			void goToFirstValid();

			ComponentMask _mask;
			std::weak_ptr<EntityManager> _manager;
			
		};

		class EntityView {
		public:
			EntityView(const EntityView &view) = default;

			EntityView &operator=(const EntityView &view) = default;

			Iterator begin();
			Iterator end();

			const Iterator begin() const;
			const Iterator end() const;

		private:
			friend EntityManager;

			EntityView() = default;
			EntityView(std::shared_ptr<EntityManager> manager, const ComponentMask &mask);

			ComponentMask _mask;
			std::weak_ptr<EntityManager> _manager;
		};

		template<typename ... Ts>
		EntityView getEntitiesWith()
		{
			return EntityView(shared_from_this(), createComponentMask<Ts ...>());
		}

	private:
		friend EntityView;
		friend Iterator;
	};

}
}

#endif
