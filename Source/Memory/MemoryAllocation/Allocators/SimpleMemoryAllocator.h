#pragma once

#include "BaseMemoryAllocator.h"

class SimpleMemoryAllocator : public BaseMemoryAllocator
{};

typedef MemoryManager<SimpleMemoryAllocator> MemManager;