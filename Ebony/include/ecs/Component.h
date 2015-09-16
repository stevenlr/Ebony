#ifndef EBONY_ECS_COMPONENT_H_
#define EBONY_ECS_COMPONENT_H_

#include <memory>

#include "Entity.h"

namespace ebony { namespace ecs {

	class EntityManager;

	template<typename T>
	class Component {
	public:
		Component() = default;
		Component(const Component<T> &component) = default;
		Component(const Entity &entity, T *component) :
			_entity(entity), _component(component)
		{}

		Component<T> &operator=(const Component<T> &component) = default;
		bool operator==(const Component<T> &component)
		{
			return component._entity == _entity && component._component == _component;
		}

		inline T *operator->() { return _component; }
		inline const T *operator->() const { return _component; }

		inline T *get() { return _component; }
		inline const T *get() const { return _component; }

		operator bool()
		{
			return _entity && _component && *this == _entity.getComponent<T>();
		}

		void remove()
		{
			if (*this) {
				_entity.removeComponent<T>();
			}
		}

	private:
		Entity _entity;
		T *_component;
	};

}
}

#endif
