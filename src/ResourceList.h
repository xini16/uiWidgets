#ifndef RESOURCELIST_H
#define RESOURCELIST_H

#include "src/ResourceTreeItem.h"
#include <QDragEnterEvent>
#include <QDragMoveEvent>
#include <QDropEvent>
#include <QTreeWidget>

template <typename T> class ResourceList : public QTreeWidget {
  Q_OBJECT

public:
  explicit ResourceList(QWidget *parent = nullptr);

signals:
  void itemDropped(QTreeWidgetItem *target, ResourceTreeItem<T> *dragged);

protected:
  void dragEnterEvent(QDragEnterEvent *event) override;
  void dragMoveEvent(QDragMoveEvent *event) override;
  void dropEvent(QDropEvent *event) override;
};

#endif // RESOURCELIST_H
