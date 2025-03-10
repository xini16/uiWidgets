#include "ResourceViewUI.h"
#include "ResourceManager.h"
#include <QCursor>
#include <QInputDialog>
#include <QMenu>
#include <QMessageBox>
#include <iostream>

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
    QTreeWidgetItem *item = new QTreeWidgetItem(resourceList);
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
    QTreeWidgetItem *childItem = new QTreeWidgetItem(parentItem);
    childItem->setText(0, QString::fromStdString(child->getName()));
    parentItem->addChild(insertBeforeItem);
    parentItem->addChild(childItem);

    populateTree(childItem, child);
  }
}

void ResourceViewUI::onResourceSelected() {
  if (resourceList->currentItem()) {
    Resource *selectedResource =
        getResourceFromItem(resourceList->currentItem());
    if (selectedResource) {
      emit resourceSelected(selectedResource);
    }
  }
}

void ResourceViewUI::handleItemDrop(QTreeWidgetItem *target,
                                    QTreeWidgetItem *dragged) {
  assert(target);
  assert(dragged);
  Resource *draggedResource = getResourceFromItem(dragged);
  assert(draggedResource);

  QTreeWidgetItem *parentItem = target->parent();
  Resource *parent = getResourceFromItem(parentItem);
  int index = parentItem->indexOfChild(target);
  std::cout << "dragged resource has parent " << draggedResource->getParent()
            << std::endl;
  resourceManager->removeParent(draggedResource);
  resourceManager->insertChild(parent, draggedResource, index);
  updateView();
}

// TODO QTreeWidgetItem
// ！！与resource互相查找的方式可以更加直接，建议每个resource记录自己的QTreeWidgetItem，然后给QTreeWidget、QTreeWidgetItem写子类，可以直接携带Resource*
// 这样做的话这个下面的几个function可以写的很短很短~运行效率也好（不用挨个比对string）
QTreeWidgetItem *ResourceViewUI::findInsertPointForItem(QTreeWidgetItem *parent,
                                                        Resource *resource) {
  assert(parent);
  for (int i = 0; i < parent->childCount(); i++) {
    QTreeWidgetItem *child = parent->child(i);

    if (getResourceFromItem(child) == resource && i > 0) {
      QTreeWidgetItem *insertPoint = parent->child(i - 1);
      if (insertPoint->text(0).isEmpty()) {
        return insertPoint;
      }
    }

    QTreeWidgetItem *found = findInsertPointForItem(child, resource);
    if (found)
      return found;
  }

  return nullptr; // TODO  avoid doing this
}

Resource *ResourceViewUI::getResourceFromItem(QTreeWidgetItem *item) {
  if (!item) {
    return resourceManager->getRoot();
  }
  for (Resource *resource : resourceManager->getRoot()->getChildren()) {
    if (resource->getName() == item->text(0).toStdString()) {
      return resource;
    }
    Resource *childResource =
        findResourceInChildren(resource, item->text(0).toStdString());
    if (childResource) {
      return childResource;
    }
  }
  assert(false);
}

Resource *ResourceViewUI::findResourceInChildren(Resource *parent,
                                                 const std::string &name) {
  for (Resource *child : parent->getChildren()) {
    if (child->getName() == name) {
      return child;
    }
    Resource *result = findResourceInChildren(child, name);
    if (result) {
      return result;
    }
  }
  return nullptr; // TODO  avoid doing this, never return nullptr
}

void ResourceViewUI::showContextMenu(const QPoint &pos) {
  QTreeWidgetItem *selectedItem = resourceList->itemAt(pos);
  if (!selectedItem)
    return;

  QMenu contextMenu;

  // wait for further enhancements

  contextMenu.exec(resourceList->mapToGlobal(pos));
}
