#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H
#include "Resource.h"
#include <QObject>

class ResourceManager : public QObject {
  Q_OBJECT

public:
  explicit ResourceManager(QObject *parent = nullptr);
  Resource *getRoot() { return root; }
public slots:
  void addResource(Resource *parent, const std::string &name,
                   const ResourceType type);
  void deleteResource(Resource *resource);
  void renameResource(Resource *resource, const std::string &newName);
  void sortResources(const std::string &criteria, const SortOrder &order);
  void insertChild(Resource *parent, Resource *child, std::size_t index);
  void removeParent(Resource *child);
  void createTestData();

signals:
  void resourceUpdated();

private:
  Resource *root = nullptr;
};

#endif // RESOURCEMANAGER_H
