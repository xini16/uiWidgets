#ifndef MENUBARUI_H
#define MENUBARUI_H

#include "ResourceViewUI.h"
#include "fruitExample.h"
#include <QActionGroup>
#include <QHBoxLayout>
#include <QInputDialog>
#include <QLineEdit>
#include <QMenu>
#include <QMessageBox>
#include <QPushButton>
#include <QToolButton>
#include <QWidget>
#include <optional>
#include <qtreewidget.h>

class Resource;

class ResourceManager;
class MenuBarUI : public QWidget {
  Q_OBJECT

public:
  explicit MenuBarUI(ResourceManager *resourceManager,
                     std::unordered_map<int, std::function<void *()>> map = {},
                     QWidget *parent = nullptr);
  std::optional<Resource *> selectedResource = {};
  std::optional<QTreeWidgetItem *> selectedInsertPoint = {};
  int indexOfTopLevel = 0;

signals:
  void addResource(Resource *parent, const std::string &name, void *item);
  void insertNewResource(Resource *parent, const std::string &name, void *item,
                         std::size_t index);
  void renameResource(Resource *resource, const std::string &newName);
  void sortResources(const std::string &criteria, const SortOrder &order);
  void deleteResource(Resource *resource);
  void searchResource(const QString &searchText);

public slots:
  void onDeleteResource();
  void onRenameResource();
  void onSearchTextChanged(const QString &text);

private:
  ResourceManager *resourceManager = nullptr;
  std::unordered_map<int, std::function<void *()>> map = {};
  QPushButton *addButton = nullptr;
  QToolButton *sortButton = nullptr;
  std::string criteria = "name";
  void sortbuttonClicked();
  SortOrder order = None;
  QLineEdit *searchBox = nullptr;
  QPushButton *renameButton = nullptr;
  QPushButton *deleteButton = nullptr;
  QMenu *addMenu = nullptr;
};

#endif // MENUBARUI_H
