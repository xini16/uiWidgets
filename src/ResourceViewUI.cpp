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

ResourceViewUI::ResourceViewUI(ResourceManager *resourceManager,
                               QWidget *parent)
    : QWidget(parent), resourceManager(resourceManager) {
  mainLayout = new QVBoxLayout(this);

  resourceList = new ResourceList(this);

  mainLayout->addWidget(resourceList);
  setLayout(mainLayout);
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
  // originalItemOrder.clear();
  // for (int i = 0; i < resourceList->topLevelItemCount(); ++i) {
  //   originalItemOrder.push_back(resourceList->topLevelItem(i));
  // }
  repaintPage();
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
    emit insertPointSelected(resourceList->currentItem());
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

  if (item) {
    Resource *clickedResource = item->getResource();
    assert(clickedResource);
    QAction *copyAction = contextMenu.addAction("Copy");
    QAction *pasteAction = contextMenu.addAction("Paste");
    QAction *newAction = contextMenu.addAction("New");
    QMenu *newSubMenu = new QMenu("Type", &contextMenu);
    QAction *type1Action = newSubMenu->addAction("Type A");
    QAction *type2Action = newSubMenu->addAction("Type B");
    QAction *type3Action = newSubMenu->addAction("Type C");
    newAction->setMenu(newSubMenu);
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
      ResourceTreeItem *parentItem =
          dynamic_cast<ResourceTreeItem *>(clickedInsertPoint->parent());
      Resource *parent = parentItem->getResource();
      int index = parentItem->indexOfChild(clickedInsertPoint) / 2;
      QAction *pasteAction = contextMenu.addAction("Paste");
      QAction *newAction = contextMenu.addAction("New");
      QMenu *newSubMenu = new QMenu("Type", &contextMenu);
      QAction *type1Action = newSubMenu->addAction("Type A");
      QAction *type2Action = newSubMenu->addAction("Type B");
      QAction *type3Action = newSubMenu->addAction("Type C");
      newAction->setMenu(newSubMenu);

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
      QAction *pasteAction = contextMenu.addAction("Paste");
      QAction *newAction = contextMenu.addAction("New");
      QMenu *newSubMenu = new QMenu("Type", &contextMenu);
      QAction *type1Action = newSubMenu->addAction("Type A");
      QAction *type2Action = newSubMenu->addAction("Type B");
      QAction *type3Action = newSubMenu->addAction("Type C");
      newAction->setMenu(newSubMenu);

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
  // if (order == None)
  //   repaintPage();
  // std::vector<Resource *> resources =
  // resourceManager->getRoot()->getChildren();

  // auto compareByName = [](Resource *a, Resource *b) {
  //   return a->getName() < b->getName();
  // };

  // auto compareByTag = [](Resource *a, Resource *b) {
  //   return a->getTag() < b->getTag();
  // };

  // if (criteria == "name") {
  //   std::sort(resources.begin(), resources.end(), compareByName);
  // } else if (criteria == "tag") {
  //   std::sort(resources.begin(), resources.end(), compareByTag);
  // } else {
  //   assert(false);
  // }

  // if (order == Descending) {
  //   std::reverse(resources.begin(), resources.end());
  // }

  // repaintPage();
}
