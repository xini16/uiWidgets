#include "ResourceList.h"

ResourceList::ResourceList(QWidget *parent)
    : QTreeWidget(parent) {
    setAcceptDrops(true);
    setDragEnabled(true);
    setDropIndicatorShown(true);
    viewport()->setAcceptDrops(true);
    setHeaderHidden(true);
    setSelectionMode(QAbstractItemView::SingleSelection);
    setContextMenuPolicy(Qt::CustomContextMenu);
    setDefaultDropAction(Qt::MoveAction);
}

void ResourceList::dragEnterEvent(QDragEnterEvent *event) {
        event->acceptProposedAction();
    }

void ResourceList::dragMoveEvent(QDragMoveEvent *event) {
    static QTreeWidgetItem *previousHighlightedInsert = nullptr; 
    static QTreeWidgetItem *previousHighlightedResource = nullptr;

    QTreeWidgetItem *itemUnderCursor = itemAt(event->position().toPoint());
    if (previousHighlightedInsert && previousHighlightedInsert->treeWidget()) {
        previousHighlightedInsert->setBackground(0, Qt::gray);
    }

    if (previousHighlightedResource && previousHighlightedResource->treeWidget()) {
        previousHighlightedResource->setBackground(0, Qt::transparent);
    }

    previousHighlightedInsert = nullptr;
    previousHighlightedResource = nullptr;

    if (itemUnderCursor && itemUnderCursor->text(0).isEmpty()) {
        itemUnderCursor->setBackground(0, Qt::yellow);
        previousHighlightedInsert = itemUnderCursor;
    }
    else if (itemUnderCursor) {
        itemUnderCursor->setBackground(0, QColor(255, 255, 150)); 
        previousHighlightedResource = itemUnderCursor;
    }

    event->acceptProposedAction();
}

void ResourceList::dropEvent(QDropEvent *event) {
    QTreeWidgetItem *itemUnderCursor = itemAt(event->position().toPoint());
    if (!itemUnderCursor) {
        return;
    }
    QTreeWidgetItem *draggedItem = currentItem();
    if (!draggedItem) {
        return;
    }

    emit itemDropped(itemUnderCursor, draggedItem);
    
    event->setDropAction(Qt::MoveAction);
    event->accept();
}

