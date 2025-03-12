#ifndef RESOURCEVIEWUI_H
#define RESOURCEVIEWUI_H

#include "ResourceList.h"
#include "ResourceTreeItem.h"
#include <QDragMoveEvent>
#include <QDropEvent>
#include <QMenu>
#include <QMimeData>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>

class Resource;
class ResourceManager;

class ResourceViewUI : public QWidget {
  Q_OBJECT

public:
  explicit ResourceViewUI(ResourceManager *resourceManager,
                          QWidget *parent = nullptr);

signals:
  void resourceSelected(Resource *resource);
private slots:
  void updateView();
  void showContextMenu(const QPoint &pos);
  void onResourceSelected();
  void handleItemDrop(QTreeWidgetItem *target, ResourceTreeItem *dragged);

private:
  void populateTree(QTreeWidgetItem *parentItem, Resource *resource);
  // Resource *getResourceFromItem(QTreeWidgetItem *item);
  // Resource *findResourceInChildren(Resource *parent, const std::string
  // &name);
  ResourceManager *resourceManager;
  QVBoxLayout *mainLayout;
  ResourceList *resourceList;
  Resource *selectedResource;
};

#endif // RESOURCEVIEWUI_H
