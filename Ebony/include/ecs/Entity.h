#ifndef EBONY_ECS_ENTITY_H_
#define EBONY_ECS_ENTITY_H_

#include <memory>

#include "constants.h"

namespace ebony { namespace ecs {

	class EntityManager;

	template<typename T>
	class Component;

	class Entity {
	public:
		Entity() = default;
		Entity(std::shared_ptr<EntityManager> manager, EntityId id, EntityVersion version);
		Entity(const Entity &entity) = default;

		Entity &operator=(const Entity &entity) = default;
		bool operator==(const Entity &entity) const;
		bool operator!=(const Entity &entity) const;
		operator bool() const;

		bool isValid() const;
		void destroy();

		template<typename T>
		Component<T> addComponent()
		{
			if (!isValid()) {
				return Component<T>();
			}

			return _manager.lock()->addComponent<T>(*this);
		}

		template<typename T>
		Component<T> getComponent()
		{
			if (!isValid()) {
				return Component<T>();
			}

			return _manager.lock()->getComponent<T>(*this);
		}

		template<typename T>
		void removeComponent()
		{
			if (!isValid()) {
				return;
			}

			_manager.lock()->removeComponent<T>(*this);
		}

		template<typename T>
		bool hasComponent()
		{
			if (!isValid()) {
				return false;
			}

			return _manager.lock()->hasComponents<T>(*this);
		}

		template<typename T1, typename T2, typename ... Ts>
		bool hasComponents()
		{
			if (!isValid()) {
				return false;
			}

			return _manager.lock()->hasComponents<T1, T2, Ts ...>(*this);
		}

	private:
		friend EntityManager;

		std::weak_ptr<EntityManager> _manager;
		EntityId _id;
		EntityVersion _version;
	};

}
}

#endif
