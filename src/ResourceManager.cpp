#include "ResourceManager.h"
#include "Resource.h"
#include "src/types.h"
#include <iostream>

ResourceManager::ResourceManager(QObject *parent)
    : QObject(parent), root(new Resource("root", (void *)nullptr, false)) {}

void ResourceManager::addResource(Resource *parent, const std::string &name,
                                  void *type) {
  assert(parent);
  Resource *newResource = new Resource(name, type, true);
  parent->addChild(newResource);
  emit resourceUpdated();
}

void ResourceManager::deleteResource(Resource *resource) {
  resource->getParent()->removeChild(resource);
  delete resource;
  emit resourceUpdated();
}

void ResourceManager::renameResource(Resource *resource,
                                     const std::string &newName) {
  if (resource) {
    resource->setName(newName);
    emit resourceUpdated();
  }
}

void ResourceManager::removeParent(Resource *child) {
  assert(child);
  std::cout << "Child " << child->getName() << "@" << child
            << " removing parent " << child->getParent() << std::endl;
  assert(child->getParent());
  child->removeParent();
  emit resourceUpdated();
}

void ResourceManager::insertChild(Resource *parent, Resource *child,
                                  std::size_t index) {
  assert(child);
  assert(parent);
  std::cout << "Inserting " << child->getName() << " at " << parent->getName()
            << "-" << index << std::endl;
  parent->insertChild(child, index);
  emit resourceUpdated();
}

void ResourceManager::insertNewResource(Resource *parent,
                                        const std::string &name, void *type,
                                        std::size_t index) {
  assert(parent);
  Resource *newResource = new Resource(name, type, true);
  insertChild(parent, newResource, index);
}

Resource *ResourceManager::copyResource(Resource *resource) {
  Resource *newResource = new Resource(resource->getName(), resource->getItem(),
                                       resource->isLeaf());
  if (resource->hasChildren()) {
    for (Resource *child : resource->getChildren()) {
      Resource *copiedChild = copyResource(child);
      newResource->addChild(copiedChild);
    }
  }
  return newResource;
}

void ResourceManager::addFolder(Resource *parent, const std::string &name) {
  assert(parent);
  Resource *newFolder = new Resource(name, nullptr, false);
  parent->addChild(newFolder);
  emit resourceUpdated();
}

void ResourceManager::insertFolder(Resource *parent, const std::string &name,
                                   int index) {
  assert(parent);
  Resource *newFolder = new Resource(name, nullptr, false);
  insertChild(parent, newFolder, index);
}
