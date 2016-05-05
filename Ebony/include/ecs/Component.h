#ifndef EBONY_ECS_COMPONENT_H_
#define EBONY_ECS_COMPONENT_H_

#include <memory>

namespace ebony { namespace ecs {

class Entity;
class EntityManager;

template<typename T>
class Component;

template<typename T0, typename T1>
bool operator==(const Component<T0> &c0, const Component<T1> &c1);

template<typename T0, typename T1>
bool operator!=(const Component<T0> &c0, const Component<T1> &c1);

template<typename T>
class Component {
public:
	Component() = default;
	Component(const Component<T> &component) = default;
	Component(const Entity &entity, T *component);

	Component<T> &operator=(const Component<T> &component) = default;

	template<typename T0, typename T1>
	friend bool operator==(const Component<T0> &c0, const Component<T1> &c1);

	template<typename T0, typename T1>
	friend bool operator!=(const Component<T0> &c0, const Component<T1> &c1);

	inline T *operator->() { return _component; }
	inline const T *operator->() const { return _component; }

	inline T *get() { return _component; }
	inline const T *get() const { return _component; }

	operator bool();
	void remove();

private:
	Entity		_entity;
	T		*	_component;
};

}}
	
#include "Entity.h"
#include "EntityManager.h"

namespace ebony { namespace ecs {

template<typename T>
Component<T>::Component(const Entity &entity, T *component) :
	_entity(entity), _component(component)
{}

template<typename T0, typename T1>
bool operator==(const Component<T0> &c0, const Component<T1> &c1)
{
	return std::is_same<T0, T1>::value
		&& static_cast<void *>(c0._component) == static_cast<void *>(c1._component)
		&& c0._entity == c1._entity;
}

template<typename T0, typename T1>
bool operator!=(const Component<T0> &c0, const Component<T1> &c1)
{
	return !(c0 == c1);
}
	
template<typename T>
Component<T>::operator bool()
{
	return _entity && _component && *this == _entity.getComponent<T>();
}
	
template<typename T>
void Component<T>::remove()
{
	if (*this) {
		_entity.removeComponent<T>();
	}
}

}}

#endif
