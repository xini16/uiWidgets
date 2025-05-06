#include "MenuBarUI.h"
#include "ResourceManager.h"
#include "ResourceViewUI.h"
#include <iostream>
#include <qtreewidget.h>

MenuBarUI::MenuBarUI(ResourceManager *resourceManager,
                     ResourceViewUI *resourceViewUI,
                     std::unordered_map<int, std::function<void *()>> map,
                     QWidget *parent)
    : QWidget(parent), map(map), resourceManager(resourceManager),
      resourceViewUI(resourceViewUI) {
  QHBoxLayout *layout = new QHBoxLayout(this);

  addButton = new QPushButton(this);
  addButton->setText("Add file");
  addMenu = new QMenu(this);
  for (auto [type, typeName] : theMap.left) {
    QAction *action = addMenu->addAction(QString::fromStdString(typeName));

    connect(action, &QAction::triggered, this, [=, this]() {
      std::optional<Resource *> currentResource =
          resourceViewUI->getSelectedResource();
      std::optional<QTreeWidgetItem *> currentInsertPoint =
          resourceViewUI->selectedInsertPoint;
      if (currentResource) {
        if (!currentResource.value()->isLeaf())
          emit addResource(currentResource.value(), "New Resource " + typeName,
                           map.at(static_cast<int>(type))());
        else
          QMessageBox::warning(this, tr("Error"),
                               tr("Cannot add new file into a file"));
        return;
      } else if (currentInsertPoint) {
        ResourceTreeItem *parentItem = dynamic_cast<ResourceTreeItem *>(
            currentInsertPoint.value()->parent());
        Resource *parent =
            parentItem ? parentItem->getResource() : resourceManager->getRoot();
        int index =
            parentItem
                ? getIndex(parentItem->indexOfChild(currentInsertPoint.value()))
                : getIndex(resourceViewUI->resourceList->indexOfTopLevelItem(
                      resourceViewUI->resourceList->currentItem()));
        emit insertNewResource(parent, "New Resource " + typeName,
                               map.at(static_cast<int>(type))(), index);
        return;
      } else {
        emit addResource(resourceManager->getRoot(), "New Resource " + typeName,
                         map.at(static_cast<int>(type))());
        return;
      }
      assert(false);
    });
  }
  connect(addButton, &QPushButton::clicked, this, [=, this]() {
    addMenu->exec(addButton->mapToGlobal(QPoint(0, addButton->height())));
  });
  addFolderButton = new QPushButton(this);
  addFolderButton->setText("Add folder");
  connect(addFolderButton, &QPushButton::clicked, this, [=, this]() {
    std::optional<Resource *> currentResource =
        resourceViewUI->getSelectedResource();
    std::optional<QTreeWidgetItem *> currentInsertPoint =
        resourceViewUI->selectedInsertPoint;
    if (currentResource) {
      if (!currentResource.value()->isLeaf())
        emit addFolder(currentResource.value(), "New Folder");
      else
        QMessageBox::warning(this, tr("Error"),
                             tr("Cannot add new folder inside a file"));
      return;
    } else if (currentInsertPoint) {
      ResourceTreeItem *parentItem = dynamic_cast<ResourceTreeItem *>(
          currentInsertPoint.value()->parent());
      Resource *parent =
          parentItem ? parentItem->getResource() : resourceManager->getRoot();
      int index =
          parentItem
              ? getIndex(parentItem->indexOfChild(currentInsertPoint.value()))
              : getIndex(resourceViewUI->resourceList->indexOfTopLevelItem(
                    resourceViewUI->resourceList->currentItem()));
      emit insertFolder(parent, "New Folder", index);
      return;
    } else {
      emit addFolder(resourceManager->getRoot(), "New Folder");
      return;
    }
    assert(false);
  });
  QMenu *sortHoverMenu = new QMenu(this);
  QActionGroup *sortGroup = new QActionGroup(this);
  sortGroup->setExclusive(true);
  QAction *name = sortHoverMenu->addAction("name");
  name->setCheckable(true);
  QAction *tag = sortHoverMenu->addAction("tag");
  tag->setCheckable(true);
  sortGroup->addAction(name);
  sortGroup->addAction(tag);

  sortButton = new QToolButton(this);
  sortButton->setText("Sort");
  sortButton->setMenu(sortHoverMenu);
  sortButton->setPopupMode(QToolButton::MenuButtonPopup);
  order = None;
  connect(name, &QAction::triggered, this, [=, this]() {
    criteria = "name";
    emit sortResources(criteria, order);
  });
  connect(tag, &QAction::triggered, this, [=, this]() {
    criteria = "tag";
    emit sortResources(criteria, order);
  });
  connect(sortButton, &QPushButton::clicked, this,
          [=, this]() { sortbuttonClicked(); });

  searchBox = new QLineEdit(this);
  searchBox->setPlaceholderText("Search...");
  layout->addWidget(searchBox);
  connect(searchBox, &QLineEdit::editingFinished, this, [this]() {
    QString text = searchBox->text();
    this->onSearchTextChanged(text);
  });

  renameButton = new QPushButton("Rename", this);
  connect(renameButton, &QPushButton::clicked, this,
          &MenuBarUI::onRenameResource);

  deleteButton = new QPushButton("Delete", this);
  connect(deleteButton, &QPushButton::clicked, this,
          &MenuBarUI::onDeleteResource);

  layout->addWidget(addButton);
  layout->addWidget(addFolderButton);
  layout->addWidget(sortButton);
  layout->addWidget(renameButton);
  layout->addWidget(deleteButton);
  setLayout(layout);
  connect(this, &MenuBarUI::addResource, resourceManager,
          &ResourceManager::addResource);
  connect(this, &MenuBarUI::addFolder, resourceManager,
          &ResourceManager::addFolder);
  connect(this, &MenuBarUI::renameResource, resourceManager,
          &ResourceManager::renameResource);
  connect(this, &MenuBarUI::sortResources, resourceViewUI,
          &ResourceViewUI::sortResources);
  connect(this, &MenuBarUI::deleteResource, resourceManager,
          &ResourceManager::deleteResource);
  connect(this, &MenuBarUI::insertNewResource, resourceManager,
          &ResourceManager::insertNewResource);
  connect(this, &MenuBarUI::insertFolder, resourceManager,
          &ResourceManager::insertFolder);
  connect(this, &MenuBarUI::searchResource, resourceViewUI,
          &ResourceViewUI::filterResources);
}

MenuBarUI::~MenuBarUI() {
  delete addButton;
  delete addFolderButton;
  delete sortButton;
  delete searchBox;
  delete renameButton;
  delete deleteButton;
  delete addMenu;
}

void MenuBarUI::sortbuttonClicked() {
  switch (order) {
  case None:
    order = Ascending;
    break;
  case Ascending:
    order = Descending;
    break;
  case Descending:
    order = None;
    break;
  default:
    assert(false);
  }
  std::string nextOrderText = sortOrder.left.at(order);
  sortButton->setText(QString::fromStdString(nextOrderText));
  emit sortResources(criteria, order);
}

void MenuBarUI::onDeleteResource() {
  for (Resource *res : resourceViewUI->selectedResources) {
    resourceManager->deleteResource(res);
  }
  resourceViewUI->selectedResources.clear();
}

void MenuBarUI::onRenameResource() {
  std::optional<Resource *> currentResource =
      resourceViewUI->getSelectedResource();
  if (currentResource) {
    bool inputFinished;
    QString newName = QInputDialog::getText(
        this, tr("Rename Resource"), tr("Enter new name for the resource:"),
        QLineEdit::Normal, "", &inputFinished);
    if (inputFinished && !newName.isEmpty()) {
      std::string stdNewName = newName.toStdString();
      emit renameResource(currentResource.value(), stdNewName);
      currentResource = {};
    } else {
      QMessageBox::warning(this, tr("Error"), tr("Invalid input"));
      return;
    }
  }
}

void MenuBarUI::onSearchTextChanged(const QString &text) {
  emit searchResource(text);
}
