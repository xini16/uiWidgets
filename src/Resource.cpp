#include "Resource.h"
#include <algorithm>
#include <cassert>
#include <iostream>

template <typename T>
Resource<T>::Resource(const std::string &name, T *item)
    : name(name), item(item), parent(nullptr) {}

template <typename T> Resource<T>::~Resource() {
  for (Resource *child : children) {
    delete child;
  }
}

template <typename T> std::string Resource<T>::getName() const { return name; }

template <typename T> std::string Resource<T>::getTag() const { return tag; }

template <typename T> T *Resource<T>::getItem() const { return item; }

template <typename T>
std::vector<Resource<T> *> Resource<T>::getChildren() const {
  return children;
}

template <typename T> Resource<T> *Resource<T>::getParent() const {
  return parent;
}

template <typename T> void Resource<T>::setParent(Resource<T> *newParent) {
  parent = newParent;
}

template <typename T> bool Resource<T>::isFolder() const {
  return !children.empty();
}

template <typename T> void Resource<T>::setName(const std::string &name) {
  this->name = name;
}

template <typename T> void Resource<T>::setTag(const std::string &tag) {
  this->tag = tag;
}

template <typename T> void Resource<T>::setItem(T *item) { this->item = item; }

template <typename T> void Resource<T>::addChild(Resource<T> *child) {
  assert(child->getParent() == nullptr);
  children.push_back(child);
  child->setParent(this);
}

template <typename T> void Resource<T>::removeChild(Resource<T> *child) {
  assert(child);
  assert(child->getParent() == this);
  children.erase(std::remove(children.begin(), children.end(), child),
                 children.end());
  child->setParent(nullptr);
}

template <typename T> bool Resource<T>::hasChildren() const {
  return !children.empty();
}

template <typename T>
void Resource<T>::insertChild(Resource<T> *child, std::size_t index) {
  assert(child);
  assert(!child->getParent());
  children.insert(children.begin() + index, child);
  child->setParent(this);
}

template <typename T> void Resource<T>::removeParent() {
  std::cout << "Resource::removeParent()" << std::endl;
  parent->removeChild(this);
}
