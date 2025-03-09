#ifndef RESOURCEVIEWUI_H
#define RESOURCEVIEWUI_H

#include <QWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include <QDragMoveEvent> 
#include <QDropEvent>   
#include <QMimeData>
#include "ResourceList.h"
#include <QMenu>
class Resource;
class ResourceManager;

class ResourceViewUI : public QWidget {
    Q_OBJECT

public:
    explicit ResourceViewUI(ResourceManager *resourceManager, QWidget *parent = nullptr);

signals:
    void resourceSelected(Resource* resource);
    void removeChild(Resource* resource);
    void insertChild(Resource* parent, Resource* child, std::size_t index);

private slots:
    void updateView();        
    void showContextMenu(const QPoint &pos); 
    void onResourceSelected();
    void handleItemDrop(QTreeWidgetItem *target, QTreeWidgetItem *dragged);


private:
    void populateTree(QTreeWidgetItem *parentItem, Resource *resource); 
    Resource* getResourceFromItem(QTreeWidgetItem *item);
    Resource* findResourceInChildren(Resource *parent, const std::string &name); 
    QTreeWidgetItem* findInsertPointForItem(QTreeWidgetItem *parent, Resource *resource);
    ResourceManager *resourceManager;
    QVBoxLayout *mainLayout;
    ResourceList *resourceList;
    Resource* selectedResource;
};

#endif // RESOURCEVIEWUI_H
