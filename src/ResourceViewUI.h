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
#include <qtreewidget.h>
#include <unordered_map>

class Resource;
class ResourceManager;

class ResourceViewUI : public QWidget {
  Q_OBJECT

public:
  explicit ResourceViewUI(ResourceManager *resourceManager,
                          QWidget *parent = nullptr);

signals:
  void resourceSelected(Resource *resource);
  void insertPointSelected(QTreeWidgetItem *insertPoint);

private slots:
  void updateView();
  void showContextMenu(const QPoint &pos);
  void onItemSelected();
  void handleItemDrop(QTreeWidgetItem *target, ResourceTreeItem *dragged);

private:
  void populateTree(QTreeWidgetItem *parentItem, Resource *resource);
  void recordExpandedStateFromTree(QTreeWidgetItem *item);
  void restoreExpandedStateFromTree(QTreeWidgetItem *item);
  ResourceManager *resourceManager;
  QVBoxLayout *mainLayout;
  ResourceList *resourceList;
  Resource *selectedResource;
  std::unordered_map<Resource *, bool> expandedStateMap;
};

#endif // RESOURCEVIEWUI_H
