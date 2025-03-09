#ifndef MENUBARUI_H
#define MENUBARUI_H

#include <QWidget>
#include <QPushButton>
#include <QToolButton>
#include <QHBoxLayout>
#include <QMenu>
#include <QInputDialog>
#include <QLineEdit>
#include <QMessageBox>
#include "utensil.h"

class Resource;
class ResourceManager;

class MenuBarUI : public QWidget {
    Q_OBJECT

public:
    explicit MenuBarUI(QWidget *parent = nullptr);
    Resource* selectedResource;

signals:
    void addResource(Resource *parent, const std::string &name, const ResourceType type);
    void renameResource(Resource *resource, const std::string &newName);
    void sortResources(const std::string &criteria, const sortOrder &order);
    void deleteResource(Resource *resource);

public slots:
    void onDeleteResource();
    void onRenameResource();

private:
    QToolButton *addButton;
    ResourceType lastAddedType;
    QToolButton *sortButton;
    std::string criteria;
    void sortbuttonClicked();
    sortOrder order;
    QPushButton *renameButton;
    QPushButton *deleteButton;
    QMenu *addMenu;

};

#endif // MENUBARUI_H
