#include "ResourceManager.h"
#include "src/types.h"
#include <iostream>

template <typename T>
ResourceManager<T>::ResourceManager(QObject *parent)
    : QObject(parent), root(new Resource<T>("root", TypeA)) {}

template <typename T>
void ResourceManager<T>::addResource(Resource<T> *parent,
                                     const std::string &name, const T &type) {
  assert(parent);
  Resource<T> *newResource = new Resource<T>(name, type);
  parent->addChild(newResource);
  emit resourceUpdated();
}

template <typename T>
void ResourceManager<T>::deleteResource(Resource<T> *resource) {
  resource->getParent()->removeChild(resource);
  delete resource;
  emit resourceUpdated();
}

template <typename T>
void ResourceManager<T>::renameResource(Resource<T> *resource,
                                        const std::string &newName) {
  if (resource) {
    resource->setName(newName);
    emit resourceUpdated();
  }
}

template <typename T>
void ResourceManager<T>::removeParent(Resource<T> *child) {
  assert(child);
  std::cout << "Child " << child->getName() << "@" << child
            << " removing parent " << child->getParent() << std::endl;
  assert(child->getParent());
  child->removeParent();
  emit resourceUpdated();
}

template <typename T>
void ResourceManager<T>::insertChild(Resource<T> *parent, Resource<T> *child,
                                     std::size_t index) {
  assert(child);
  assert(parent);
  std::cout << "Inserting " << child->getName() << " at " << parent->getName()
            << "-" << index << std::endl;
  parent->insertChild(child, index);
  emit resourceUpdated();
}

template <typename T>
void ResourceManager<T>::insertNewResource(Resource<T> *parent,
                                           const std::string &name,
                                           const T &type, std::size_t index) {
  assert(parent);
  Resource<T> *newResource = new Resource<T>(name, type);
  insertChild(parent, newResource, index);
}

template <typename T>
Resource<T> *ResourceManager<T>::copyResource(Resource<T> *resource) {
  Resource<T> *newResource =
      new Resource<T>(resource->getName(), resource->getType());
  if (resource->hasChildren()) {
    for (Resource<T> *child : resource->getChildren()) {
      Resource<T> *copiedChild = copyResource(child);
      newResource->addChild(copiedChild);
    }
  }
  return newResource;
}

// template <typename T> void ResourceManager<T>::createTestData() {
//   Resource<T> *folder1 = new Resource<T>("Folder A", TypeA);
//   Resource<T> *folder2 = new Resource<T>("Folder B", TypeB);

//   root->addChild(folder1);
//   root->addChild(folder2);

//   Resource<T> *file1 = new Resource<T>("File A1", TypeA);
//   Resource<T> *file2 = new Resource<T>("File B1", TypeB);

//   folder1->addChild(file1);
//   folder2->addChild(file2);

//   std::cout << file1->getName() << "@" << file1 << " has parent "
//             << file1->getParent() << "@" << folder1 << std::endl;

//   std::cout << file2->getName() << "@" << file2 << " has parent "
//             << file2->getParent() << "@" << folder2 << std::endl;

//   emit resourceUpdated();
// }
