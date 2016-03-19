#ifndef EBONY_TESTS_ECS_ECS_TEST_H_
#define EBONY_TESTS_ECS_ECS_TEST_H_

#include <gtest/gtest.h>

#include "ecs/ecs.h"

namespace ebony { namespace tests {

class ECSEntityTest : public testing::Test {
protected:
	ECSEntityTest();

	std::shared_ptr<ecs::EntityManager> manager1;
	std::shared_ptr<ecs::EntityManager> manager2;
};

class ECSComponentTest : public testing::Test {
protected:
	ECSComponentTest();

	std::shared_ptr<ecs::EntityManager> manager;
	ecs::Entity e1;
	ecs::Entity e2;
};

struct ComponentA {
	const char type = 'A';
	int data;
};

struct ComponentB {
	const char type = 'B';
	int data;
};

}}

#endif
