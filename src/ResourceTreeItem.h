#ifndef RESOURCETREEITEM_H
#define RESOURCETREEITEM_H

#include "Resource.h"
#include <QTreeWidgetItem>

template <typename T> class ResourceTreeItem : public QTreeWidgetItem {
public:
  explicit ResourceTreeItem(Resource<T> *resource,
                            QTreeWidget *parent = nullptr)
      : QTreeWidgetItem(parent), resource(resource) {}

  explicit ResourceTreeItem(Resource<T> *resource, QTreeWidgetItem *parent)
      : QTreeWidgetItem(parent), resource(resource) {}

  Resource<T> *getResource() const { return resource; }

private:
  Resource<T> *resource;
};

#endif // RESOURCETREEITEM_H
