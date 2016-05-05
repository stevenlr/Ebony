#include "stdafx.h"
#include "ecs/EntityView.h"

#include "ecs/EntityManager.h"
#include "utils/assert.h"

using namespace std;

namespace ebony { namespace ecs {

EntityView_Iterator::EntityView_Iterator(const EntityView_Iterator &it) :
	_mask(it._mask), _current(it._current), _manager(it._manager)
{
	goToFirstValid();
}

EntityView_Iterator &EntityView_Iterator::operator=(const EntityView_Iterator &it)
{
	_mask = it._mask;
	_manager = it._manager;
	_current = it._current;
	goToFirstValid();

	return *this;
}

EntityView_Iterator::EntityView_Iterator(weak_ptr<EntityManager> manager,
											const ComponentMask &mask,
											EntityId current) :
	_mask(mask), _manager(manager), _current(current)
{
	goToFirstValid();
}

bool EntityView_Iterator::operator==(const EntityView_Iterator &it)
{
	return _mask == it._mask
		&& _current == it._current
		&& _manager.lock() == it._manager.lock();
}

bool EntityView_Iterator::operator!=(const EntityView_Iterator &it)
{
	return _mask != it._mask
		|| _current != it._current
		|| _manager.lock() != it._manager.lock();
}

Entity EntityView_Iterator::operator*() const
{
	return Entity(_manager, _current, _manager.lock()->_entityVersion[_current]);
}

EntityView_Iterator &EntityView_Iterator::operator++()
{
	if (_current < _manager.lock()->_nextEntity) {
		_current++;
		goToFirstValid();
	}

	return *this;
}

void EntityView_Iterator::goToFirstValid()
{
	shared_ptr<EntityManager> manager = _manager.lock();

	while (_current < manager->_nextEntity) {
		if ((manager->_componentMasks[_current] & _mask) == _mask) {
			break;
		}

		_current++;
	}
}

EntityView::EntityView(shared_ptr<EntityManager> manager,
										const ComponentMask &mask) :
	_mask(mask), _manager(manager)
{}

EntityView::Iterator EntityView::begin()
{
	return Iterator(_manager, _mask);
}

EntityView::Iterator EntityView::end()
{
	ASSERT(_manager.lock(), "Reference to manager expired");
	return Iterator(_manager, _mask, _manager.lock()->_nextEntity);
}

const EntityView::Iterator EntityView::begin() const
{
	return Iterator(_manager, _mask);
}

const EntityView::Iterator EntityView::end() const
{
	ASSERT(_manager.lock(), "Reference to manager expired");
	return Iterator(_manager, _mask, _manager.lock()->_nextEntity);
}

}}
