#ifndef TYPES_H
#define TYPES_H
enum ResourceType { TypeA, TypeB, TypeC };

enum sortOrder { None, Ascending, Descending };
// sort Order注意类型大小写哈， 应该是SortOrder
// enum 最好有对应的boost::bimap与string互换~~
#endif
