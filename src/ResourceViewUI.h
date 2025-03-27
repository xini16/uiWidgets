#ifndef RESOURCEVIEWUI_H
#define RESOURCEVIEWUI_H

#include "ResourceList.h"
#include "ResourceTreeItem.h"
#include "fruit.h"
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

template <typename T> class Resource;
template <typename T> class ResourceManager;

template <typename T> class ResourceViewUI : public QWidget {
  Q_OBJECT

public:
  explicit ResourceViewUI(ResourceManager<T> *resourceManager,
                          QWidget *parent = nullptr);

signals:
  void resourceSelected(Resource<T> *resource);
  void insertPointSelected(QTreeWidgetItem *insertPoint);

public slots:
  void filterResources(const QString &searchText);
  void sortResources(const std::string &criteria, const SortOrder &order);

private slots:
  void updateView();
  void showContextMenu(const QPoint &pos);
  void onItemSelected();
  void handleItemDrop(QTreeWidgetItem *target, ResourceTreeItem<T> *dragged);

private:
  void populateTree(QTreeWidgetItem *parentItem, Resource<T> *resource);
  void recordExpandedStateFromTree(QTreeWidgetItem *item);
  void restoreExpandedStateFromTree(QTreeWidgetItem *item);
  void filterTreeItem(QTreeWidgetItem *item, const QString &searchText);
  void repaintPage();
  ResourceManager<T> *resourceManager;
  QVBoxLayout *mainLayout;
  ResourceList<T> *resourceList;
  std::optional<Resource<T> *> selectedResource = {};
  std::unordered_map<Resource<T> *, bool> expansionStateMap;
  std::optional<Resource<T> *> clipboardResource = {};
};

#endif // RESOURCEVIEWUI_H
