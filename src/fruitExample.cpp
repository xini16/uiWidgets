#include "fruitExample.h"
std::unordered_map<int, std::function<void *()>> fruitFactoryMap = {
    {Apple,
     []() -> void * { return reinterpret_cast<void *>(new Fruit("Apple")); }},
    {Banana,
     []() -> void * { return reinterpret_cast<void *>(new Fruit("Banana")); }},
    {Orange,
     []() -> void * { return reinterpret_cast<void *>(new Fruit("Orange")); }}};