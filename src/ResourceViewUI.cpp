#include "ResourceViewUI.h"
#include <QInputDialog>
#include <QMessageBox>
#include <QMenu>
#include <QCursor>
#include "ResourceManager.h"
#include "iostream"

ResourceViewUI::ResourceViewUI(ResourceManager *resourceManager, QWidget *parent) 
    : QWidget(parent), resourceManager(resourceManager) {
    mainLayout = new QVBoxLayout(this);

    resourceList = new ResourceList(this);

    mainLayout->addWidget(resourceList);
    setLayout(mainLayout);
    connect(resourceList, &ResourceList::customContextMenuRequested, this, &ResourceViewUI::showContextMenu);
    connect(resourceList, &ResourceList::itemClicked, this, &ResourceViewUI::onResourceSelected);
    connect(resourceList, &ResourceList::itemDropped, this, &ResourceViewUI::handleItemDrop);
    connect(resourceManager, &ResourceManager::resourceUpdated, this, &ResourceViewUI::updateView);
    updateView();
}

void ResourceViewUI::updateView() {
    resourceList->clear();
    std::cout<<"updated"<<std::endl;
    for (auto *resource : resourceManager->getRootResources()) {
        QTreeWidgetItem *insertBeforeItem = new QTreeWidgetItem(resourceList);
        insertBeforeItem->setFlags(insertBeforeItem->flags() & ~Qt::ItemIsSelectable); 
        insertBeforeItem->setSizeHint(0, QSize(0, 2)); 
        insertBeforeItem->setBackground(0, QBrush(Qt::gray));
        QTreeWidgetItem *item = new QTreeWidgetItem(resourceList);
        item->setText(0, QString::fromStdString(resource->getName()));
        // std::cout << "updateView -> rootresource=" << resource << std::endl;
        populateTree(item, resource); 
    }
}

void ResourceViewUI::populateTree(QTreeWidgetItem *parentItem, Resource *resource) {
    std::cout<<"populateTree called for resource:"
             << resource->getName()
             << "  children count =" << resource->getChildren().size()<<"\n";
    for (auto *child : resource->getChildren()) {
        // std::cout << "populateTree -> child=" << resource << std::endl;
        std::cout<<"Child of"
                 << resource->getName()
                 << ":" << child->getName()<<"\n";
        if (!child) continue;
        QTreeWidgetItem *insertBeforeItem = new QTreeWidgetItem(parentItem);
        insertBeforeItem->setFlags(insertBeforeItem->flags() & ~Qt::ItemIsSelectable);
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
        Resource *selectedResource = getResourceFromItem(resourceList->currentItem());
        if (selectedResource) {
            emit resourceSelected(selectedResource); 
        }
    }
}

void ResourceViewUI::handleItemDrop(QTreeWidgetItem *target, QTreeWidgetItem *dragged) {
    if (!target || !dragged) return;
    Resource *draggedResource = getResourceFromItem(dragged);
    if (!draggedResource) return;

    if (target->text(0).isEmpty()) {
        QTreeWidgetItem *parentItem = target->parent();
        Resource *parent = getResourceFromItem(parentItem);
        int index = parentItem ? parentItem->indexOfChild(target) : resourceList->indexOfTopLevelItem(target);

        emit insertChild(parent, draggedResource, index);
        emit removeChild(draggedResource);
        }

    else {
        Resource *targetResource = getResourceFromItem(target);
        if (targetResource) {
            emit insertChild(targetResource, draggedResource, 1);
            emit removeChild(draggedResource);
        }
    }

    for (int i = 0; i < resourceList->topLevelItemCount(); i++) {
        QTreeWidgetItem *item = resourceList->topLevelItem(i);
        if (item->text(0).isEmpty()) {
            item->setBackground(0, Qt::gray);
        }
    }

    updateView(); 
}

QTreeWidgetItem* ResourceViewUI::findInsertPointForItem(QTreeWidgetItem *parent, Resource *resource) {
    if (!parent) return nullptr;

    for (int i = 0; i < parent->childCount(); i++) {
        QTreeWidgetItem *child = parent->child(i);

        if (getResourceFromItem(child) == resource && i > 0) {
            QTreeWidgetItem *insertPoint = parent->child(i - 1);
            if (insertPoint->text(0).isEmpty()) {
                return insertPoint;
            }
        }

        QTreeWidgetItem *found = findInsertPointForItem(child, resource);
        if (found) return found;
    }

    return nullptr;
}

Resource* ResourceViewUI::getResourceFromItem(QTreeWidgetItem *item) {
    if (!item) return nullptr;

    for (auto *resource : resourceManager->getRootResources()) {
        if (resource->getName() == item->text(0).toStdString()) {
            return resource;
        }
        Resource *childResource = findResourceInChildren(resource, item->text(0).toStdString());
        if (childResource) {
            return childResource;
        }
    }
    return nullptr;
}

Resource* ResourceViewUI::findResourceInChildren(Resource *parent, const std::string &name) {
    for (auto *child : parent->getChildren()) {
        if (child->getName() == name) {
            return child;
        }
        Resource *result = findResourceInChildren(child, name);
        if (result) {
            return result;
        }
    }
    return nullptr;
}

void ResourceViewUI::showContextMenu(const QPoint &pos) {
    QTreeWidgetItem *selectedItem = resourceList->itemAt(pos);
    if (!selectedItem) return;

    QMenu contextMenu;
    
// wait for further enhancements

    contextMenu.exec(resourceList->mapToGlobal(pos));
}

