#include "Resource.h"
#include <algorithm>
#include <iostream>

Resource::Resource(const std::string &name, const ResourceType type, Resource *parent)
    : name(name), type(type), parent(parent) {}

Resource::~Resource() {
    for (Resource* child : children) {
        delete child;
    }
}

std::string Resource::getName() const {
    return name;
}

std::string Resource::getTag() const {
    return tag;
}

ResourceType Resource::getType() const {
    return type;
}

std::vector<Resource *> Resource::getChildren() const {
    return children;
}

Resource *Resource::getParent() const {
    return parent;
}

bool Resource::isFolder() const {
    return !children.empty();
}

void Resource::setName(const std::string &name) {
    this->name = name;
}

void Resource::setTag(const std::string &tag) {
    this->tag = tag;
}

void Resource::setType(ResourceType newType) {
    type = newType;
}

void Resource::setParent(Resource *newParent) {
    parent = newParent;
}

void Resource::addChild(Resource *child) {
    if (std::find(children.begin(), children.end(), child) == children.end()) {
        children.push_back(child);
        child->parent = this;
    }
}

void Resource::removeChild(Resource *child) {
    if (!child) return; 

    auto it = std::find(children.begin(), children.end(), child);
    if (it != children.end()) {
        children.erase(it);
        if (child->getParent() == this) {
            child->setParent(nullptr);
        }
    }
}    

bool Resource::hasChildren() const {
    return !children.empty();
}

void Resource::insertChild(Resource *child, std::size_t index) {
    if (!child) return;
    if (index > children.size()) {
        index = children.size();
    }
    auto it = std::find(children.begin(), children.end(), child);
    if (it != children.end()) {
        children.erase(it);
    }
    children.insert(children.begin() + index, child);
    child->setParent(this);
}