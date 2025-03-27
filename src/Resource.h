#ifndef RESOURCE_H
#define RESOURCE_H
#include "types.h"
#include <string>
#include <vector>

template <typename T> class Resource {
public:
  Resource(const std::string &name, T *item);
  ~Resource();
  std::string getName() const;
  std::string getTag() const;
  std::vector<Resource *> getChildren() const;
  Resource *getParent() const;
  T *getItem() const;
  bool isFolder() const;

  void setName(const std::string &name);
  void setTag(const std::string &tag);
  void setItem(T *item);
  void setParent(Resource<T> *newParent);
  void addChild(Resource<T> *child);
  void removeChild(Resource<T> *child);
  bool hasChildren() const;
  void insertChild(Resource<T> *child, std::size_t index);
  void removeParent();

private:
  std::string name;
  std::string tag;
  Resource<T> *parent = nullptr;
  std::vector<Resource<T> *> children = {};
  T *item;
};

#endif // RESOURCE_H
