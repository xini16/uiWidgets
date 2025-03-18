#ifndef MENUBARUI_H
#define MENUBARUI_H

#include "ResourceViewUI.h"
#include "types.h"
#include <QHBoxLayout>
#include <QInputDialog>
#include <QLineEdit>
#include <QMenu>
#include <QMessageBox>
#include <QPushButton>
#include <QToolButton>
#include <QWidget>
#include <qtreewidget.h>

class Resource;
class ResourceManager;

class MenuBarUI : public QWidget {
  Q_OBJECT

public:
  explicit MenuBarUI(QWidget *parent = nullptr);
  std::optional<Resource *> selectedResource;
  std::optional<QTreeWidgetItem *> selectedInsertPoint;

signals:
  void addResource(Resource *parent, const std::string &name,
                   const ResourceType type);
  void insertNewResource(Resource *parent, const std::string &name,
                         const ResourceType type, std::size_t index);
  void renameResource(Resource *resource, const std::string &newName);
  void sortResources(const std::string &criteria, const SortOrder &order);
  void deleteResource(Resource *resource);

public slots:
  void onDeleteResource();
  void onRenameResource();

private:
  QToolButton *addButton;
  ResourceType lastAddedType = TypeA;
  QToolButton *sortButton;
  std::string criteria = "name";
  void sortbuttonClicked();
  SortOrder order = None;
  QPushButton *renameButton;
  QPushButton *deleteButton;
  QMenu *addMenu;
};

#endif // MENUBARUI_H
