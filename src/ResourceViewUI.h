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
#include <algorithm>
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

public slots:
  void filterResources(const QString &searchText);
  void sortResources(const std::string &criteria, const SortOrder &order);

private slots:
  void updateView();
  void showContextMenu(const QPoint &pos);
  void onItemSelected();
  void handleItemDrop(QTreeWidgetItem *target, ResourceTreeItem *dragged);

private:
  void populateTree(QTreeWidgetItem *parentItem, Resource *resource);
  void recordExpandedStateFromTree(QTreeWidgetItem *item);
  void restoreExpandedStateFromTree(QTreeWidgetItem *item);
  void filterTreeItem(QTreeWidgetItem *item, const QString &searchText);
  void repaintPage();
  Resource *copyResource(Resource *resource);

  ResourceManager *resourceManager;
  QVBoxLayout *mainLayout;
  ResourceList *resourceList;
  std::optional<Resource *> selectedResource = {};
  std::unordered_map<Resource *, bool> expansionStateMap;
  std::optional<Resource *> clipboardResource = {};
  // std::vector<QTreeWidgetItem *> originalItemOrder;
};

#endif // RESOURCEVIEWUI_H
