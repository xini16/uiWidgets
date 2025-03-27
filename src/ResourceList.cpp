#include "ResourceList.h"
#include "ResourceTreeItem.h"

template <typename T>
ResourceList<T>::ResourceList(QWidget *parent) : QTreeWidget(parent) {
  setAcceptDrops(true);
  setDragEnabled(true);
  setDropIndicatorShown(true);
  viewport()->setAcceptDrops(true);
  setHeaderHidden(true);
  setSelectionMode(QAbstractItemView::SingleSelection);
  setContextMenuPolicy(Qt::CustomContextMenu);
  setDefaultDropAction(Qt::MoveAction);
}

template <typename T>
void ResourceList<T>::dragEnterEvent(QDragEnterEvent *event) {
  event->acceptProposedAction();
}

template <typename T>
void ResourceList<T>::dragMoveEvent(QDragMoveEvent *event) {
  static QTreeWidgetItem *previousHighlightedInsert = nullptr;
  static QTreeWidgetItem *previousHighlightedResource = nullptr;

  QTreeWidgetItem *itemUnderCursor = itemAt(event->position().toPoint());
  if (previousHighlightedInsert && previousHighlightedInsert->treeWidget()) {
    previousHighlightedInsert->setBackground(0, Qt::gray);
  }

  if (previousHighlightedResource &&
      previousHighlightedResource->treeWidget()) {
    previousHighlightedResource->setBackground(0, Qt::transparent);
  }

  previousHighlightedInsert = nullptr;
  previousHighlightedResource = nullptr;

  if (itemUnderCursor && itemUnderCursor->text(0).isEmpty()) {
    itemUnderCursor->setBackground(0, Qt::yellow);
    previousHighlightedInsert = itemUnderCursor;
  } else if (itemUnderCursor) {
    itemUnderCursor->setBackground(0, QColor(255, 255, 150));
    previousHighlightedResource = itemUnderCursor;
  }

  event->acceptProposedAction();
}

template <typename T> void ResourceList<T>::dropEvent(QDropEvent *event) {
  QTreeWidgetItem *itemUnderCursor = itemAt(event->position().toPoint());
  ResourceTreeItem<T> *draggedItem =
      dynamic_cast<ResourceTreeItem<T> *>(currentItem());
  assert(draggedItem);

  emit itemDropped(itemUnderCursor, draggedItem);

  event->setDropAction(Qt::MoveAction);
  event->accept();
}
