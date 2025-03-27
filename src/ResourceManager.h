#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H
#include "Resource.h"
#include <QObject>

template <typename T> class ResourceManager : public QObject {
  Q_OBJECT
public:
  explicit ResourceManager(QObject *parent = nullptr);
  Resource<T> *getRoot() { return root; }

public slots:
  void addResource(Resource<T> *parent, const std::string &name, const T &type);
  void insertNewResource(Resource<T> *parent, const std::string &name,
                         const T &type, std::size_t index);
  void deleteResource(Resource<T> *resource);
  void renameResource(Resource<T> *resource, const std::string &newName);
  void insertChild(Resource<T> *parent, Resource<T> *child, std::size_t index);
  Resource<T> *copyResource(Resource<T> *resource);
  void removeParent(Resource<T> *child);
  // void createTestData();

signals:
  void resourceUpdated();

private:
  Resource<T> *root = nullptr;
};

#endif // RESOURCEMANAGER_H
