#include "Resource.h"
#include <algorithm>
#include <cassert>
#include <iostream>

Resource::Resource(const std::string &name, const ResourceType type)
    : name(name), type(type) {}

Resource::~Resource() {
  for (Resource *child : children) {
    delete child;
  }
}

std::string Resource::getName() const { return name; }

std::string Resource::getTag() const { return tag; }

ResourceType Resource::getType() const { return type; }

std::vector<Resource *> Resource::getChildren() const { return children; }

Resource *Resource::getParent() const { return parent; }

void Resource::setParent(Resource *newParent) { parent = newParent; }

bool Resource::isFolder() const { return !children.empty(); }

void Resource::setName(const std::string &name) { this->name = name; }

void Resource::setTag(const std::string &tag) { this->tag = tag; }

void Resource::setType(ResourceType newType) { type = newType; }

void Resource::addChild(Resource *child) {
  assert(child->getParent() == nullptr);
  children.push_back(child);
  child->setParent(this);
}

void Resource::removeChild(Resource *child) {
  std::cout << "Resource::removeChild()" << std::endl;
  assert(child);
  assert(child->getParent() == this);
  children.erase(std::remove(children.begin(), children.end(), child),
                 children.end());
  child->setParent(nullptr);
  std::cout << "-----------------" << std::endl;
}

bool Resource::hasChildren() const { return !children.empty(); }

void Resource::insertChild(Resource *child, std::size_t index) {
  assert(child);
  assert(!child->getParent());
  children.insert(children.begin() + index, child);
  child->setParent(this);
}

void Resource::removeParent() {
  std::cout << "Resource::removeParent()" << std::endl;
  parent->removeChild(this);
}
