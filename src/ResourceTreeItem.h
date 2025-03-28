#ifndef RESOURCETREEITEM_H
#define RESOURCETREEITEM_H

#include "Resource.h"
#include <QTreeWidgetItem>

class ResourceTreeItem : public QTreeWidgetItem {
public:
  explicit ResourceTreeItem(Resource *resource, QTreeWidget *parent = nullptr)
      : QTreeWidgetItem(parent), resource(resource) {}

  explicit ResourceTreeItem(Resource *resource, QTreeWidgetItem *parent)
      : QTreeWidgetItem(parent), resource(resource) {}

  Resource *getResource() const { return resource; }

private:
  Resource *resource;
};

#endif // RESOURCETREEITEM_H
