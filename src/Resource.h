#ifndef RESOURCE_H
#define RESOURCE_H
#include "types.h"
#include <string>
#include <vector>

class Resource {
public:
  Resource(const std::string &name, ResourceType type);
  ~Resource();
  std::string getName() const;
  std::string getTag() const;
  std::vector<Resource *> getChildren() const;
  Resource *getParent() const;
  ResourceType getType() const;
  bool isFolder() const;

  void setName(const std::string &name);
  void setTag(const std::string &tag);
  void setType(ResourceType type);
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
  ResourceType type;
};

#endif // RESOURCE_H
