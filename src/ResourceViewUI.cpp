#include "ResourceViewUI.h"
#include "ResourceManager.h"
#include "src/ResourceTreeItem.h"
#include "src/types.h"
#include <QCursor>
#include <QInputDialog>
#include <QMenu>
#include <QMessageBox>
#include <iostream>
#include <qnamespace.h>
#include <qtreewidget.h>

template <typename T>
ResourceViewUI<T>::ResourceViewUI(ResourceManager<T> *resourceManager,
                                  QWidget *parent)
    : QWidget(parent), resourceManager(resourceManager) {
  mainLayout = new QVBoxLayout(this);

  resourceList = new ResourceList(this);

  mainLayout->addWidget(resourceList);
  setLayout(mainLayout);
  connect(resourceList, &ResourceList<T>::customContextMenuRequested, this,
          &ResourceViewUI::showContextMenu);
  connect(resourceList, &ResourceList<T>::itemClicked, this,
          &ResourceViewUI::onItemSelected);
  connect(resourceList, &ResourceList<T>::itemDropped, this,
          &ResourceViewUI::handleItemDrop);
  connect(resourceManager, &ResourceManager<T>::resourceUpdated, this,
          &ResourceViewUI::updateView);
  updateView();
}

template <typename T> void ResourceViewUI<T>::updateView() {
  expansionStateMap.clear();
  for (int i = 0; i < resourceList->topLevelItemCount(); ++i) {
    recordExpandedStateFromTree(resourceList->topLevelItem(i));
  }
  repaintPage();
}

template <typename T> void ResourceViewUI<T>::repaintPage() {
  resourceList->clear();
  for (Resource<T> *resource : resourceManager->getRoot()->getChildren()) {
    QTreeWidgetItem *insertBeforeItem = new QTreeWidgetItem(resourceList);
    insertBeforeItem->setFlags(insertBeforeItem->flags() |
                               Qt::ItemIsSelectable);
    insertBeforeItem->setSizeHint(0, QSize(0, 2));
    insertBeforeItem->setBackground(0, QBrush(Qt::gray));
    ResourceTreeItem<T> *item = new ResourceTreeItem(resource, resourceList);
    item->setText(0, QString::fromStdString(resource->getName()));
    populateTree(item, resource);
  }
  for (int i = 0; i < resourceList->topLevelItemCount(); ++i) {
    restoreExpandedStateFromTree(resourceList->topLevelItem(i));
  }
}

template <typename T>
void ResourceViewUI<T>::populateTree(QTreeWidgetItem *parentItem,
                                     Resource<T> *resource) {
  for (Resource<T> *child : resource->getChildren()) {
    QTreeWidgetItem *insertBeforeItem = new QTreeWidgetItem(parentItem);
    insertBeforeItem->setFlags(insertBeforeItem->flags() |
                               Qt::ItemIsSelectable);
    insertBeforeItem->setSizeHint(0, QSize(0, 2));
    insertBeforeItem->setBackground(0, QBrush(Qt::gray));
    ResourceTreeItem<T> *childItem = new ResourceTreeItem(child, parentItem);
    childItem->setText(0, QString::fromStdString(child->getName()));
    parentItem->addChild(insertBeforeItem);
    parentItem->addChild(childItem);

    populateTree(childItem, child);
  }
}

template <typename T> void ResourceViewUI<T>::onItemSelected() {
  ResourceTreeItem<T> *item =
      dynamic_cast<ResourceTreeItem<T> *>(resourceList->currentItem());
  if (item) {
    Resource<T> *selectedResource = item->getResource();
    assert(selectedResource);
    emit resourceSelected(selectedResource);
  } else {
    emit insertPointSelected(resourceList->currentItem());
  }
}

template <typename T>
void ResourceViewUI<T>::handleItemDrop(QTreeWidgetItem *target,
                                       ResourceTreeItem<T> *dragged) {
  assert(target);
  assert(dragged);
  Resource<T> *draggedResource = dragged->getResource();
  assert(draggedResource);
  if (target->text(0).isEmpty()) {
    ResourceTreeItem<T> *parentItem =
        dynamic_cast<ResourceTreeItem<T> *>(target->parent());
    Resource<T> *parent = parentItem->getResource();
    int index = parentItem->indexOfChild(target) / 2;
    std::cout << "dragged resource has parent " << draggedResource->getParent()
              << std::endl;
    resourceManager->removeParent(draggedResource);
    resourceManager->insertChild(parent, draggedResource, index);
  } else {
    ResourceTreeItem<T> *targetResource =
        dynamic_cast<ResourceTreeItem<T> *>(target);
    assert(targetResource);
    Resource<T> *parent = targetResource->getResource();
    assert(parent);
    int index = parent->getChildren().size();
    resourceManager->removeParent(draggedResource);
    resourceManager->insertChild(parent, draggedResource, index);
  }
  updateView();
}

template <typename T>
void ResourceViewUI<T>::recordExpandedStateFromTree(QTreeWidgetItem *item) {
  if (!item)
    return;
  ResourceTreeItem<T> *rti = dynamic_cast<ResourceTreeItem<T> *>(item);
  if (rti && rti->getResource()) {
    expansionStateMap[rti->getResource()] =
        resourceList->isExpanded(resourceList->indexFromItem(rti));
  }
  for (int i = 0; i < item->childCount(); ++i) {
    recordExpandedStateFromTree(item->child(i));
  }
}

template <typename T>
void ResourceViewUI<T>::restoreExpandedStateFromTree(QTreeWidgetItem *item) {
  if (!item)
    return;
  ResourceTreeItem<T> *rti = dynamic_cast<ResourceTreeItem<T> *>(item);
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

template <typename T>
void ResourceViewUI<T>::showContextMenu(const QPoint &pos) {
  QMenu contextMenu;
  ResourceTreeItem<T> *item =
      dynamic_cast<ResourceTreeItem<T> *>(resourceList->itemAt(pos));
  QAction *pasteAction = contextMenu.addAction("Paste");
  QAction *newAction = contextMenu.addAction("New");
  QMenu *newSubMenu = new QMenu("Type", &contextMenu);
  QAction *type1Action = newSubMenu->addAction("Type A");
  QAction *type2Action = newSubMenu->addAction("Type B");
  QAction *type3Action = newSubMenu->addAction("Type C");
  newAction->setMenu(newSubMenu);
  if (item) {
    Resource<T> *clickedResource = item->getResource();
    assert(clickedResource);
    QAction *copyAction = contextMenu.addAction("Copy");
    QAction *deleteAction = contextMenu.addAction("Delete");
    QAction *renameAction = contextMenu.addAction("Rename");
    QAction *cutAction = contextMenu.addAction("Cut");

    connect(copyAction, &QAction::triggered, this, [=]() {
      clipboardResource = resourceManager->copyResource(clickedResource);
    });
    connect(pasteAction, &QAction::triggered, this, [=]() {
      resourceManager->insertChild(clickedResource, clipboardResource.value(),
                                   clickedResource->getChildren().size());
    });
    connect(type1Action, &QAction::triggered, this, [=]() {
      resourceManager->addResource(clickedResource, "New Resource of Type A",
                                   TypeA);
    });
    connect(type2Action, &QAction::triggered, this, [=]() {
      resourceManager->addResource(clickedResource, "New Resource of Type B",
                                   TypeB);
    });
    connect(type3Action, &QAction::triggered, this, [=]() {
      resourceManager->addResource(clickedResource, "New Resource of Type C",
                                   TypeC);
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
      ResourceTreeItem<T> *parentItem =
          dynamic_cast<ResourceTreeItem<T> *>(clickedInsertPoint->parent());
      Resource<T> *parent = parentItem->getResource();
      int index = parentItem->indexOfChild(clickedInsertPoint) / 2;

      connect(pasteAction, &QAction::triggered, this, [=]() {
        resourceManager->insertChild(parent, clipboardResource.value(), index);
      });
      connect(type1Action, &QAction::triggered, this, [=]() {
        resourceManager->insertNewResource(parent, "New Resource of Type A",
                                           TypeA, index);
      });
      connect(type2Action, &QAction::triggered, this, [=]() {
        resourceManager->insertNewResource(parent, "New Resource of Type B",
                                           TypeB, index);
      });
      connect(type3Action, &QAction::triggered, this, [=]() {
        resourceManager->insertNewResource(parent, "New Resource of Type C",
                                           TypeC, index);
      });
    } else {
      connect(pasteAction, &QAction::triggered, this, [=]() {
        if (clipboardResource) {
          resourceManager->insertChild(
              resourceManager->getRoot(), clipboardResource.value(),
              resourceManager->getRoot()->getChildren().size());
        }
      });
      connect(type1Action, &QAction::triggered, this, [=]() {
        resourceManager->addResource(resourceManager->getRoot(),
                                     "New Resource of Type A", TypeA);
      });
      connect(type2Action, &QAction::triggered, this, [=]() {
        resourceManager->addResource(resourceManager->getRoot(),
                                     "New Resource of Type B", TypeB);
      });
      connect(type3Action, &QAction::triggered, this, [=]() {
        resourceManager->addResource(resourceManager->getRoot(),
                                     "New Resource of Type C", TypeC);
      });
    }
  }

  contextMenu.exec(resourceList->mapToGlobal(pos));
}

template <typename T>
void ResourceViewUI<T>::filterResources(const QString &searchText) {
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

template <typename T>
void ResourceViewUI<T>::filterTreeItem(QTreeWidgetItem *item,
                                       const QString &searchText) {
  assert(item);
  item->text(0).contains(searchText, Qt::CaseInsensitive)
      ? item->setBackground(0, QBrush(Qt::yellow))
      : item->setForeground(0, QBrush(Qt::gray));
  for (int i = 0; i < item->childCount(); ++i) {
    filterTreeItem(item->child(i), searchText);
  }
}

template <typename T>
void ResourceViewUI<T>::sortResources(const std::string &criteria,
                                      const SortOrder &order) {
  if (order == None) {
    updateView();
    return;
  }
  std::vector<Resource<T> *> resources =
      resourceManager->getRoot()->getChildren();
  if (criteria == "name") {
    std::sort(resources.begin(), resources.end(),
              [order](Resource<T> *a, Resource<T> *b) {
                if (order == Ascending) {
                  return a->getName() < b->getName();
                } else if (order == Descending) {
                  return a->getName() > b->getName();
                }
                return false;
              });
  } else if (criteria == "tag") {
    std::sort(resources.begin(), resources.end(),
              [order](Resource<T> *a, Resource<T> *b) {
                if (order == Ascending) {
                  return a->getTag() < b->getTag();
                } else if (order == Descending) {
                  return a->getTag() > b->getTag();
                }
                return false;
              });
  }
  resourceList->clear();
  for (Resource<T> *resource : resources) {
    QTreeWidgetItem *insertBeforeItem = new QTreeWidgetItem(resourceList);
    insertBeforeItem->setFlags(insertBeforeItem->flags() |
                               Qt::ItemIsSelectable);
    insertBeforeItem->setSizeHint(0, QSize(0, 2));
    insertBeforeItem->setBackground(0, QBrush(Qt::gray));

    ResourceTreeItem<T> *item = new ResourceTreeItem(resource, resourceList);
    item->setText(0, QString::fromStdString(resource->getName()));
    populateTree(item, resource);
  }
  for (int i = 0; i < resourceList->topLevelItemCount(); ++i) {
    restoreExpandedStateFromTree(resourceList->topLevelItem(i));
  }
}
