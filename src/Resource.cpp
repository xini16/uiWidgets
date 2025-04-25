#include "Resource.h"
#include "ResourceManager.h"
#include <algorithm>
#include <cassert>
#include <iostream>

Resource::Resource(const std::string &name, void *item, bool leaf)
    : name(name), item(item), leaf(leaf), parent(nullptr) {}

Resource::~Resource() {
  for (Resource *child : children) {
    delete child;
  }
}

std::string Resource::getName() const { return name; }

std::string Resource::getTag() const { return tag; }

void *Resource::getItem() const { return item; }

std::vector<Resource *> Resource::getChildren() const { return children; }

Resource *Resource::getParent() const { return parent; }

void Resource::setParent(Resource *newParent) { parent = newParent; }

bool Resource::isLeaf() const { return leaf; }

void Resource::setName(const std::string &name) { this->name = name; }

void Resource::setTag(const std::string &tag) { this->tag = tag; }

void Resource::setItem(void *item) { this->item = item; }

void Resource::addChild(Resource *child) {
  assert(child->getParent() == nullptr);
  children.push_back(child);
  child->setParent(this);
}

void Resource::removeChild(Resource *child) {
  assert(child);
  assert(child->getParent() == this);
  children.erase(std::remove(children.begin(), children.end(), child),
                 children.end());
  child->setParent(nullptr);
}

bool Resource::hasChildren() const { return !children.empty(); }

void Resource::insertChild(Resource *child, std::size_t index) {
  assert(child);
  assert(!child->getParent());
  children.insert(children.begin() + index, child);
  child->setParent(this);
}

void Resource::removeParent() { parent->removeChild(this); }
