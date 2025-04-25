#ifndef RESOURCE_H
#define RESOURCE_H
#include "ResourceManager.h"
#include "src/ResourceManagerActual.h"
#include "types.h"
#include <string>
#include <vector>

class ResourceManager;
template <typename T> class ResourceManagerActual;
class Resource {

public:
  Resource(const std::string &name, void *item, bool leaf);
  ~Resource();
  std::string getName() const;
  std::string getTag() const;
  std::vector<Resource *> getChildren() const;
  Resource *getParent() const;
  bool isLeaf() const;
  void setName(const std::string &name);
  void setTag(const std::string &tag);
  void setItem(void *item);
  void setParent(Resource *newParent);
  void addChild(Resource *child);
  void removeChild(Resource *child);
  bool hasChildren() const;
  void insertChild(Resource *child, std::size_t index);
  void removeParent();

private:
  std::string name;
  std::string tag;
  Resource *parent = nullptr;
  std::vector<Resource *> children = {};
  void *item;
  void *getItem() const;
  bool leaf;
  friend class ResourceManager;
  template <typename T> friend class ResourceManagerActual;
};

#endif // RESOURCE_H
