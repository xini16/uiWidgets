#include "ResourceManager.h"
#include <iostream>

ResourceManager::ResourceManager(QObject *parent)
    : QObject(parent), root(new Resource("root", TypeA)) {}

void ResourceManager::addResource(Resource *parent, const std::string &name,
                                  const ResourceType type) {
  assert(parent);
  Resource *newResource = new Resource(name, type);
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

void ResourceManager::sortResources(const std::string &criteria,
                                    const SortOrder &order) {
  emit resourceUpdated();
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
                                        const std::string &name,
                                        const ResourceType type,
                                        std::size_t index) {
  assert(parent);
  Resource *newResource = new Resource(name, type);
  insertChild(parent, newResource, index);
}

void ResourceManager::createTestData() {
  Resource *folder1 = new Resource("Folder A", TypeA);
  Resource *folder2 = new Resource("Folder B", TypeB);

  root->addChild(folder1);
  root->addChild(folder2);

  Resource *file1 = new Resource("File A1", TypeA);
  Resource *file2 = new Resource("File B1", TypeB);

  folder1->addChild(file1);
  folder2->addChild(file2);

  std::cout << file1->getName() << "@" << file1 << " has parent "
            << file1->getParent() << "@" << folder1 << std::endl;

  std::cout << file2->getName() << "@" << file2 << " has parent "
            << file2->getParent() << "@" << folder2 << std::endl;

  emit resourceUpdated();
}
