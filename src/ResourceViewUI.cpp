#include "ResourceViewUI.h"
#include "ResourceManager.h"
#include "src/ResourceTreeItem.h"
#include <QCursor>
#include <QInputDialog>
#include <QMenu>
#include <QMessageBox>
#include <iostream>
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
          &ResourceViewUI::onResourceSelected);
  connect(resourceList, &ResourceList::itemDropped, this,
          &ResourceViewUI::handleItemDrop);
  connect(resourceManager, &ResourceManager::resourceUpdated, this,
          &ResourceViewUI::updateView);
  updateView();
}

void ResourceViewUI::updateView() {
  resourceList->clear();
  for (Resource *resource : resourceManager->getRoot()->getChildren()) {
    QTreeWidgetItem *insertBeforeItem = new QTreeWidgetItem(resourceList);
    insertBeforeItem->setFlags(insertBeforeItem->flags() &
                               ~Qt::ItemIsSelectable);
    insertBeforeItem->setSizeHint(0, QSize(0, 2));
    insertBeforeItem->setBackground(0, QBrush(Qt::gray));
    ResourceTreeItem *item = new ResourceTreeItem(resource, resourceList);
    item->setText(0, QString::fromStdString(resource->getName()));
    populateTree(item, resource);
  }
}

void ResourceViewUI::populateTree(QTreeWidgetItem *parentItem,
                                  Resource *resource) {
  for (Resource *child : resource->getChildren()) {
    QTreeWidgetItem *insertBeforeItem = new QTreeWidgetItem(parentItem);
    insertBeforeItem->setFlags(insertBeforeItem->flags() &
                               ~Qt::ItemIsSelectable);
    insertBeforeItem->setSizeHint(0, QSize(0, 2));
    insertBeforeItem->setBackground(0, QBrush(Qt::gray));
    ResourceTreeItem *childItem = new ResourceTreeItem(child, parentItem);
    childItem->setText(0, QString::fromStdString(child->getName()));
    parentItem->addChild(insertBeforeItem);
    parentItem->addChild(childItem);

    populateTree(childItem, child);
  }
}

void ResourceViewUI::onResourceSelected() {
  if (resourceList->currentItem()) {
    ResourceTreeItem *resourceItem =
        dynamic_cast<ResourceTreeItem *>(resourceList->currentItem());
    Resource *selectedResource = resourceItem->getResource();
    if (selectedResource) {
      emit resourceSelected(selectedResource);
    }
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
    int index = parentItem->indexOfChild(target);
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

// Resource *ResourceViewUI::getResourceFromItem(QTreeWidgetItem *item) {
//   if (!item) {
//     return resourceManager->getRoot();
//   }
//   return item->getResource();
//   assert(false);
// }

// Resource *ResourceViewUI::findResourceInChildren(Resource *parent,
//                                                  const std::string &name) {
//   for (Resource *child : parent->getChildren()) {
//     if (child->getName() == name) {
//       return child;
//     }
//     Resource *result = findResourceInChildren(child, name);
//     if (result) {
//       return result;
//     }
//   }
//   assert(false);
// }

void ResourceViewUI::showContextMenu(const QPoint &pos) {
  QTreeWidgetItem *selectedItem = resourceList->itemAt(pos);
  if (!selectedItem)
    return;

  QMenu contextMenu;

  // wait for further enhancements

  contextMenu.exec(resourceList->mapToGlobal(pos));
}
