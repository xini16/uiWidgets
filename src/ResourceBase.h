#ifndef RESOURCEBASE_H
#define RESOURCEBASE_H

#include <string>
#include <vector>

class ResourceBase {
public:
  virtual ~ResourceBase() = 0;

  virtual std::string getName() const = 0;
  virtual std::string getTag() const = 0;
  virtual ResourceBase *getParent() const = 0;
  virtual std::vector<ResourceBase *> getChildren() const = 0;
  virtual bool isFolder() const = 0;
  virtual bool hasChildren() const = 0;

  virtual void setName(const std::string &name) = 0;
  virtual void setTag(const std::string &tag) = 0;
  virtual void setParent(ResourceBase *parent) = 0;
  virtual void addChild(ResourceBase *child) = 0;
  virtual void removeChild(ResourceBase *child) = 0;
  virtual void insertChild(ResourceBase *child, size_t index) = 0;
  virtual void removeParent() = 0;

  template <typename T> T *getItemAs() const {
    return dynamic_cast<T *>(getItemVoid());
  }

protected:
  virtual void *getItemVoid() const = 0;
};

#endif // RESOURCEBASE_H