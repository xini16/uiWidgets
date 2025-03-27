#ifndef MENUBARUI_H
#define MENUBARUI_H

#include "ResourceViewUI.h"
#include "types.h"
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

template <typename T> class Resource;

template <typename T> class ResourceManager;

template <typename T> class MenuBarUI : public QWidget {
  Q_OBJECT

public:
  explicit MenuBarUI(QWidget *parent = nullptr);
  std::optional<Resource<T> *> selectedResource = {};
  std::optional<QTreeWidgetItem *> selectedInsertPoint = {};

signals:
  void addResource(Resource<T> *parent, const std::string &name,
                   const ResourceType type);
  void insertNewResource(Resource<T> *parent, const std::string &name,
                         const ResourceType type, std::size_t index);
  void renameResource(Resource<T> *resource, const std::string &newName);
  void sortResources(const std::string &criteria, const SortOrder &order);
  void deleteResource(Resource<T> *resource);
  void searchResource(const QString &searchText);

public slots:
  void onDeleteResource();
  void onRenameResource();
  void onSearchTextChanged(const QString &text);

private:
  QToolButton *addButton;
  ResourceType lastAddedType = TypeA;
  QToolButton *sortButton;
  std::string criteria = "name";
  void sortbuttonClicked();
  SortOrder order = None;
  QLineEdit *searchBox;
  QPushButton *renameButton;
  QPushButton *deleteButton;
  QMenu *addMenu;
};

#endif // MENUBARUI_H
