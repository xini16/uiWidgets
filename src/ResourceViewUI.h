#ifndef RESOURCEVIEWUI_H
#define RESOURCEVIEWUI_H

#include "ResourceList.h"
#include "ResourceTreeItem.h"
#include "fruitExample.h"
#include "utils.h"
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
  explicit ResourceViewUI(
      ResourceManager *resourceManager = nullptr,
      std::unordered_map<int, std::function<void *()>> map = {},
      QWidget *parent = nullptr);
  ~ResourceViewUI();
  std::optional<Resource *> getSelectedResource();

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
  void setupShortcuts();
  void pasteAction(std::optional<Resource *> targetItem);
  void copyAction();
  void selectWithChildren(Resource *resource);
  void deselectWithChildren(Resource *resource);
  void selectRange(Resource *start, Resource *end);
  void toggleSelection(Resource *resource);
  void updateSelectedResourceUI();
  void keyPressEvent(QKeyEvent *event) override;
  void keyReleaseEvent(QKeyEvent *event) override;

  ResourceManager *resourceManager;
  QVBoxLayout *mainLayout;
  ResourceList *resourceList;
  std::optional<Resource *> firstSelectedResource = {};
  std::set<Resource *> selectedResources = {};
  std::optional<QTreeWidgetItem *> selectedInsertPoint = {};
  bool isShiftPressed = false;
  bool isCtrlPressed = false;
  std::unordered_map<Resource *, bool> expansionStateMap;
  std::vector<Resource *> clipboardResources = {};
  std::unordered_map<int, std::function<void *()>> map;
  SortOrder sortState = None;
  std::string sortCriteria = "name";
  friend class MenuBarUI;
};

#endif // RESOURCEVIEWUI_H
