#ifndef EBONY_TESTS_ECS_ECS_TEST_H_
#define EBONY_TESTS_ECS_ECS_TEST_H_

#include <gtest/gtest.h>

#include "ecs/ecs.h"

namespace ebony { namespace tests {

class ECSTest : public testing::Test {
protected:
	ECSTest();

	std::shared_ptr<ecs::EntityManager> manager1;
	std::shared_ptr<ecs::EntityManager> manager2;
};

}}

#endif
