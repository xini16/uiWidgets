#ifndef RESOURCELIST_H
#define RESOURCELIST_H

#include <QTreeWidget>
#include <QDropEvent>
#include <QDragEnterEvent>
#include <QDragMoveEvent>

class ResourceList : public QTreeWidget {
    Q_OBJECT

public:
    explicit ResourceList(QWidget *parent = nullptr);

signals:
    void itemDropped(QTreeWidgetItem *target, QTreeWidgetItem *dragged);

protected:
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dragMoveEvent(QDragMoveEvent *event) override;
    void dropEvent(QDropEvent *event) override;
};

#endif // RESOURCELIST_H
