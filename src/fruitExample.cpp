#include "fruitExample.h"
std::unordered_map<int, std::function<void *()>> fruitFactoryMap = {
    {0,
     []() -> void * { return reinterpret_cast<void *>(new Fruit("Apple")); }},
    {1,
     []() -> void * { return reinterpret_cast<void *>(new Fruit("Banana")); }},
    {2,
     []() -> void * { return reinterpret_cast<void *>(new Fruit("Orange")); }}};