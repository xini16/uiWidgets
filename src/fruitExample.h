#ifndef FRUITEXAMP_H
#define FRUITEXAMP_H
#include "fruit.h"
#include "types.h"
#include <boost/bimap.hpp>
#include <functional>
#include <unordered_map>

enum Type { Apple, Banana, Orange };

typedef boost::bimap<Type, std::string> FruitTypeBimap;
const FruitTypeBimap theMap = makeBimap<FruitTypeBimap>(
    {{Apple, "Apple"}, {Banana, "Banana"}, {Orange, "Orange"}});

extern std::unordered_map<int, std::function<void *()>> fruitFactoryMap;

#endif