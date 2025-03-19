#ifndef TYPES_H
#define TYPES_H
#include <boost/bimap.hpp>
enum ResourceType { TypeA, TypeB, TypeC };

enum SortOrder { None, Ascending, Descending };

typedef boost::bimaps::bimap<ResourceType, std::string> ResourceTypeBimap;
typedef boost::bimaps::bimap<SortOrder, std::string> SortOrderBimap;
template <typename BimapType>
BimapType
makeBimap(std::initializer_list<typename BimapType::value_type> list) {
  return BimapType(list.begin(), list.end());
}

const ResourceTypeBimap resourceType = makeBimap<ResourceTypeBimap>(
    {{TypeA, "TypeA"}, {TypeB, "TypeB"}, {TypeC, "TypeC"}});
const SortOrderBimap sortOrder = makeBimap<SortOrderBimap>(
    {{None, "None"}, {Ascending, "Ascending"}, {Descending, "Descending"}});

#endif
