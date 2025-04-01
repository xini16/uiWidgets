#include "ResourceViewUI.h"
#include "ResourceManager.h"
#include "src/ResourceTreeItem.h"
#include "src/types.h"
#include <QCursor>
#include <QInputDialog>
#include <QKeySequence>
#include <QMenu>
#include <QMessageBox>
#include <QShortcut>
#include <iostream>
#include <qnamespace.h>
#include <qtreewidget.h>

ResourceViewUI::ResourceViewUI(
    ResourceManager *resourceManager,
    std::unordered_map<int, std::function<void *()>> map, QWidget *parent)
    : QWidget(parent), resourceManager(resourceManager), map(map) {
  mainLayout = new QVBoxLayout(this);

  resourceList = new ResourceList(this);

  mainLayout->addWidget(resourceList);
  setLayout(mainLayout);
  setupShortcuts();
  connect(resourceList, &ResourceList::customContextMenuRequested, this,
          &ResourceViewUI::showContextMenu);
  connect(resourceList, &ResourceList::itemClicked, this,
          &ResourceViewUI::onItemSelected);
  connect(resourceList, &ResourceList::itemDropped, this,
          &ResourceViewUI::handleItemDrop);
  connect(resourceManager, &ResourceManager::resourceUpdated, this,
          &ResourceViewUI::updateView);
  updateView();
}

void ResourceViewUI::updateView() {
  expansionStateMap.clear();
  for (int i = 0; i < resourceList->topLevelItemCount(); ++i) {
    recordExpandedStateFromTree(resourceList->topLevelItem(i));
  }
  repaintPage();
  if (sortState != None) {
    sortResources(sortCriteria, sortState);
  }
}

void ResourceViewUI::repaintPage() {
  resourceList->clear();
  for (Resource *resource : resourceManager->getRoot()->getChildren()) {
    QTreeWidgetItem *insertBeforeItem = new QTreeWidgetItem(resourceList);
    insertBeforeItem->setFlags(insertBeforeItem->flags() |
                               Qt::ItemIsSelectable);
    insertBeforeItem->setSizeHint(0, QSize(0, 2));
    insertBeforeItem->setBackground(0, QBrush(Qt::gray));
    ResourceTreeItem *item = new ResourceTreeItem(resource, resourceList);
    item->setText(0, QString::fromStdString(resource->getName()));
    populateTree(item, resource);
  }
  for (int i = 0; i < resourceList->topLevelItemCount(); ++i) {
    restoreExpandedStateFromTree(resourceList->topLevelItem(i));
  }
}

void ResourceViewUI::populateTree(QTreeWidgetItem *parentItem,
                                  Resource *resource) {
  for (Resource *child : resource->getChildren()) {
    QTreeWidgetItem *insertBeforeItem = new QTreeWidgetItem(parentItem);
    insertBeforeItem->setFlags(insertBeforeItem->flags() |
                               Qt::ItemIsSelectable);
    insertBeforeItem->setSizeHint(0, QSize(0, 2));
    insertBeforeItem->setBackground(0, QBrush(Qt::gray));
    ResourceTreeItem *childItem = new ResourceTreeItem(child, parentItem);
    childItem->setText(0, QString::fromStdString(child->getName()));
    parentItem->addChild(insertBeforeItem);
    parentItem->addChild(childItem);

    populateTree(childItem, child);
  }
}

void ResourceViewUI::onItemSelected() {
  ResourceTreeItem *item =
      dynamic_cast<ResourceTreeItem *>(resourceList->currentItem());
  if (item) {
    Resource *selectedResource = item->getResource();
    assert(selectedResource);
    emit resourceSelected(selectedResource);
  } else {
    emit insertPointSelected(
        resourceList->currentItem(),
        resourceList->indexOfTopLevelItem(resourceList->currentItem()) / 2);
  }
}

void ResourceViewUI::handleItemDrop(QTreeWidgetItem *target,
                                    ResourceTreeItem *dragged) {
  assert(target);
  assert(dragged);
  Resource *draggedResource = dragged->getResource();
  assert(draggedResource);
  if (target->text(0).isEmpty()) {
    ResourceTreeItem *parentItem =
        dynamic_cast<ResourceTreeItem *>(target->parent());
    Resource *parent = parentItem->getResource();
    int index = parentItem->indexOfChild(target) / 2;
    std::cout << "dragged resource has parent " << draggedResource->getParent()
              << std::endl;
    resourceManager->removeParent(draggedResource);
    resourceManager->insertChild(parent, draggedResource, index);
  } else {
    ResourceTreeItem *targetResource = dynamic_cast<ResourceTreeItem *>(target);
    assert(targetResource);
    Resource *parent = targetResource->getResource();
    assert(parent);
    int index = parent->getChildren().size();
    resourceManager->removeParent(draggedResource);
    resourceManager->insertChild(parent, draggedResource, index);
  }
  updateView();
}

void ResourceViewUI::recordExpandedStateFromTree(QTreeWidgetItem *item) {
  if (!item)
    return;
  ResourceTreeItem *rti = dynamic_cast<ResourceTreeItem *>(item);
  if (rti && rti->getResource()) {
    expansionStateMap[rti->getResource()] =
        resourceList->isExpanded(resourceList->indexFromItem(rti));
  }
  for (int i = 0; i < item->childCount(); ++i) {
    recordExpandedStateFromTree(item->child(i));
  }
}

void ResourceViewUI::restoreExpandedStateFromTree(QTreeWidgetItem *item) {
  if (!item)
    return;
  ResourceTreeItem *rti = dynamic_cast<ResourceTreeItem *>(item);
  if (rti && rti->getResource()) {
    if (expansionStateMap.find(rti->getResource()) != expansionStateMap.end() &&
        expansionStateMap[rti->getResource()]) {
      resourceList->setExpanded(resourceList->indexFromItem(rti), true);
    }
  }
  for (int i = 0; i < item->childCount(); ++i) {
    restoreExpandedStateFromTree(item->child(i));
  }
}

void ResourceViewUI::showContextMenu(const QPoint &pos) {
  QMenu contextMenu;
  ResourceTreeItem *item =
      dynamic_cast<ResourceTreeItem *>(resourceList->itemAt(pos));
  QAction *pasteAction = contextMenu.addAction("Paste");
  QAction *newAction = contextMenu.addAction("New");
  QMenu *newSubMenu = new QMenu("Type", &contextMenu);
  newAction->setMenu(newSubMenu);
  if (item) {
    Resource *clickedResource = item->getResource();
    assert(clickedResource);
    QAction *copyAction = contextMenu.addAction("Copy");
    QAction *deleteAction = contextMenu.addAction("Delete");
    QAction *renameAction = contextMenu.addAction("Rename");
    QAction *cutAction = contextMenu.addAction("Cut");
    for (const auto &entry : fruitTypeBimap.left) {
      FruitType fruitType = entry.first;
      std::string fruitName = entry.second;
      QAction *action =
          newSubMenu->addAction(QString::fromStdString(fruitName));
      connect(action, &QAction::triggered, this, [=]() {
        resourceManager->addResource(clickedResource,
                                     "New Resource " + fruitName,
                                     map.at(static_cast<int>(fruitType))());
      });
    }
    connect(copyAction, &QAction::triggered, this, [=]() {
      clipboardResource = resourceManager->copyResource(clickedResource);
    });
    connect(pasteAction, &QAction::triggered, this, [=]() {
      resourceManager->insertChild(clickedResource, clipboardResource.value(),
                                   clickedResource->getChildren().size());
    });

    connect(deleteAction, &QAction::triggered, this,
            [=]() { resourceManager->deleteResource(clickedResource); });
    connect(renameAction, &QAction::triggered, this, [=]() {
      bool inputFinished;
      QString newName = QInputDialog::getText(
          this, tr("Rename Resource"), tr("Enter new name for the resource:"),
          QLineEdit::Normal, "", &inputFinished);
      if (inputFinished && !newName.isEmpty()) {
        std::string stdNewName = newName.toStdString();
        resourceManager->renameResource(clickedResource, stdNewName);
      } else {
        QMessageBox::warning(this, tr("Error"), tr("Invalid input"));
        return;
      }
    });
    connect(cutAction, &QAction::triggered, this, [=]() {
      resourceManager->removeParent(clickedResource);
      clipboardResource = clickedResource;
    });
  } else {
    QTreeWidgetItem *clickedInsertPoint = resourceList->itemAt(pos);
    if (clickedInsertPoint) {
      ResourceTreeItem *parentItem =
          dynamic_cast<ResourceTreeItem *>(clickedInsertPoint->parent());
      Resource *parent = parentItem->getResource();
      int index = parentItem->indexOfChild(clickedInsertPoint) / 2;

      connect(pasteAction, &QAction::triggered, this, [=]() {
        resourceManager->insertChild(parent, clipboardResource.value(), index);
      });

      for (const auto &entry : fruitTypeBimap.left) {
        FruitType fruitType = entry.first;
        std::string fruitName = entry.second;
        QAction *action =
            newSubMenu->addAction(QString::fromStdString(fruitName));
        connect(action, &QAction::triggered, this, [=]() {
          resourceManager->insertNewResource(
              parent, "New Resource " + fruitName,
              map.at(static_cast<int>(fruitType))(), index);
        });
      }
    } else {
      connect(pasteAction, &QAction::triggered, this, [=]() {
        if (clipboardResource) {
          resourceManager->insertChild(
              resourceManager->getRoot(), clipboardResource.value(),
              resourceManager->getRoot()->getChildren().size());
        }
      });
      for (const auto &entry : fruitTypeBimap.left) {
        FruitType fruitType = entry.first;
        std::string fruitName = entry.second;
        QAction *action =
            newSubMenu->addAction(QString::fromStdString(fruitName));
        connect(action, &QAction::triggered, this, [=]() {
          resourceManager->addResource(resourceManager->getRoot(),
                                       "New Resource " + fruitName,
                                       map.at(static_cast<int>(fruitType))());
        });
      }
    }
  }
  contextMenu.exec(resourceList->mapToGlobal(pos));
}

void ResourceViewUI::filterResources(const QString &searchText) {
  if (searchText.isEmpty())
    repaintPage();
  else {
    updateView();
    resourceList->expandAll();
    for (int i = 0; i < resourceList->topLevelItemCount(); ++i) {
      QTreeWidgetItem *item = resourceList->topLevelItem(i);
      filterTreeItem(item, searchText);
    }
  }
}

void ResourceViewUI::filterTreeItem(QTreeWidgetItem *item,
                                    const QString &searchText) {
  assert(item);
  item->text(0).contains(searchText, Qt::CaseInsensitive)
      ? item->setBackground(0, QBrush(Qt::yellow))
      : item->setForeground(0, QBrush(Qt::gray));
  for (int i = 0; i < item->childCount(); ++i) {
    filterTreeItem(item->child(i), searchText);
  }
}

void ResourceViewUI::sortResources(const std::string &criteria,
                                   const SortOrder &order) {
  sortState = order;
  sortCriteria = criteria;
  if (order == None) {
    updateView();
    return;
  }
  std::vector<Resource *> resources = resourceManager->getRoot()->getChildren();
  if (criteria == "name") {
    std::sort(resources.begin(), resources.end(),
              [order](Resource *a, Resource *b) {
                if (order == Ascending) {
                  return a->getName() < b->getName();
                } else if (order == Descending) {
                  return a->getName() > b->getName();
                }
                return false;
              });
  } else if (criteria == "tag") {
    std::sort(resources.begin(), resources.end(),
              [order](Resource *a, Resource *b) {
                if (order == Ascending) {
                  return a->getTag() < b->getTag();
                } else if (order == Descending) {
                  return a->getTag() > b->getTag();
                }
                return false;
              });
  }
  for (int i = 0; i < resourceList->topLevelItemCount(); ++i) {
    recordExpandedStateFromTree(resourceList->topLevelItem(i));
  }
  resourceList->clear();
  for (Resource *resource : resources) {
    QTreeWidgetItem *insertBeforeItem = new QTreeWidgetItem(resourceList);
    insertBeforeItem->setFlags(insertBeforeItem->flags() |
                               Qt::ItemIsSelectable);
    insertBeforeItem->setSizeHint(0, QSize(0, 2));
    insertBeforeItem->setBackground(0, QBrush(Qt::gray));

    ResourceTreeItem *item = new ResourceTreeItem(resource, resourceList);
    item->setText(0, QString::fromStdString(resource->getName()));
    populateTree(item, resource);
  }
  for (int i = 0; i < resourceList->topLevelItemCount(); ++i) {
    restoreExpandedStateFromTree(resourceList->topLevelItem(i));
  }
}

void ResourceViewUI::setupShortcuts() {
  QShortcut *pasteShortcut = new QShortcut(QKeySequence::Paste, this);
  connect(pasteShortcut, &QShortcut::activated, this, [this]() {
    if (!clipboardResource)
      return;
    else {
      ResourceTreeItem *item =
          dynamic_cast<ResourceTreeItem *>(resourceList->currentItem());
      Resource *selectedResource = item->getResource();
      if (selectedResource) {
        resourceManager->insertChild(selectedResource,
                                     clipboardResource.value(),
                                     selectedResource->getChildren().size());
      } else {
        ResourceTreeItem *parentItem = dynamic_cast<ResourceTreeItem *>(
            resourceList->currentItem()->parent());
        Resource *parent =
            parentItem ? parentItem->getResource() : resourceManager->getRoot();
        int index =
            parentItem
                ? parentItem->indexOfChild(resourceList->currentItem()) / 2
                : resourceList->indexOfTopLevelItem(
                      resourceList->currentItem()) /
                      2;
        resourceManager->insertChild(parent, clipboardResource.value(), index);
      }
    }
  });
}