#ifndef EBONY_ECS_CONSTANTS_H_
#define EBONY_ECS_CONSTANTS_H_

#include <cstdint>
#include <bitset>

namespace ebony { namespace ecs {

#ifndef EBONY_ECS_MAX_COMPONENTS
const unsigned int MAX_COMPONENTS = 64;
#else
const unsigned int MAX_COMPONENTS = EBONY_ECS_MAX_COMPONENTS;
#endif

typedef uint32_t					EntityId;
typedef uint32_t					EntityVersion;
typedef std::bitset<MAX_COMPONENTS>	ComponentMask;

}}

#endif
