#include "ResourceManagerTemp.h"
#include "ResourceManager.h"
#include "fruit.h"
#include <iostream>

template <class T>
ResourceManagerTemp<T>::ResourceManagerTemp(ResourceManager *resourceManager)
    : resourceManager(resourceManager) {}

template <class T> void ResourceManagerTemp<T>::createTestData() {
  Resource *folder1 = new Resource("Folder A", new T("folder 1"));
  Resource *folder2 = new Resource("Folder B", new T("folder 2"));

  resourceManager->getRoot()->addChild(folder1);
  resourceManager->getRoot()->addChild(folder2);

  Resource *file1 = new Resource("File A1", new T("apple"));
  Resource *file2 = new Resource("File B1", new T("pear"));

  folder1->addChild(file1);
  folder2->addChild(file2);

  std::cout << file1->getName() << "@" << file1 << " has parent "
            << file1->getParent() << "@" << folder1 << std::endl;

  std::cout << file2->getName() << "@" << file2 << " has parent "
            << file2->getParent() << "@" << folder2 << std::endl;

  emit resourceManager->resourceUpdated();
}

template class ResourceManagerTemp<Fruit>;
