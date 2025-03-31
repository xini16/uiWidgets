#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H
#include "Resource.h"
#include <QObject>

class ResourceManager : public QObject {
  Q_OBJECT
public:
  explicit ResourceManager(QObject *parent = nullptr);
  Resource *getRoot() { return root; }
  void addResource(Resource *parent, const std::string &name, void *type);
  void insertNewResource(Resource *parent, const std::string &name, void *type,
                         std::size_t index);
  void deleteResource(Resource *resource);
  void renameResource(Resource *resource, const std::string &newName);
  void insertChild(Resource *parent, Resource *child, std::size_t index);
  Resource *copyResource(Resource *resource);
  void removeParent(Resource *child);

signals:
  void resourceUpdated();

private:
  Resource *root = nullptr;
};

#endif // RESOURCEMANAGER_H
