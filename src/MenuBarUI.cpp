#include "MenuBarUI.h"

MenuBarUI::MenuBarUI(QWidget *parent) : QWidget(parent) {
    QHBoxLayout *layout = new QHBoxLayout(this);

    addButton = new QToolButton(this);
    addButton->setText("Add");
    addMenu = new QMenu(this);
    QAction *addA = addMenu->addAction("Type A");
    QAction *addB = addMenu->addAction("Type B");
    QAction *addC = addMenu->addAction("Type C");

    connect(addA, &QAction::triggered, this, [=]() {
        lastAddedType = TypeA;
        emit addResource(nullptr, "New Resource A", TypeA);
    });

    connect(addB, &QAction::triggered, this, [=]() {
        lastAddedType = TypeB;
        emit addResource(nullptr, "New Resource B", TypeB);
    });

    connect(addC, &QAction::triggered, this, [=]() {
        lastAddedType = TypeC;
        emit addResource(nullptr, "New Resource C", TypeC);
    });
    
    connect(addButton, &QToolButton::clicked, this, [=]() {
        emit addResource(nullptr, "New Resource", lastAddedType);
    });

    addButton->setMenu(addMenu);
    addButton->setPopupMode(QToolButton::MenuButtonPopup);

    QMenu *sortHoverMenu = new QMenu(this);
    QAction *name = sortHoverMenu->addAction("name");
    QAction *tag = sortHoverMenu->addAction("tag");

    sortButton = new QToolButton(this);
    sortButton->setText("Sort");
    sortButton->setMenu(sortHoverMenu);
    sortButton->setPopupMode(QToolButton::MenuButtonPopup);
    order = None;
    connect(name, &QAction::triggered, this, [=]() {
        criteria = "name";
        emit sortResources(criteria, order);
    });
    connect(tag, &QAction::triggered, this, [=]() {
        criteria = "tag";
        emit sortResources(criteria, order);
    });
    connect(sortButton, &QPushButton::clicked, this, [=]() {
        sortbuttonClicked();
    });

    renameButton = new QPushButton("Rename", this);
    connect(renameButton, &QPushButton::clicked, this, &MenuBarUI::onRenameResource);

    deleteButton = new QPushButton("Delete", this);
    connect(deleteButton, &QPushButton::clicked, this, &MenuBarUI::onDeleteResource);

    layout->addWidget(addButton);
    layout->addWidget(sortButton);
    layout->addWidget(renameButton);
    layout->addWidget(deleteButton);
    setLayout(layout);
}

    
void MenuBarUI::sortbuttonClicked() {       
    switch (order) {
        case None:
        sortButton->setText("Ascending");
        order = Ascending;
        emit sortResources(criteria, order);
        break;
        
        case Ascending:
        sortButton->setText("Descending");
        order = Descending;
        emit sortResources(criteria, order);
        break;
    
        case Descending: 
        sortButton->setText("Sort");
        order = None;
        emit sortResources(criteria, order);
        break;
    }
}

void MenuBarUI::onDeleteResource() {
    if (selectedResource) {
        emit deleteResource(selectedResource);
        selectedResource = nullptr;
    }
}

void MenuBarUI::onRenameResource() {
    if (selectedResource) {
        bool inputFinished;
        QString newName = QInputDialog::getText(
            this,
            tr("Rename Resource"),                
            tr("Enter new name for the resource:"),
            QLineEdit::Normal,                      
            "",                                     
            &inputFinished
        );
        if (inputFinished && !newName.isEmpty()) {
            std::string stdNewName = newName.toStdString();
            emit renameResource(selectedResource, stdNewName);
            selectedResource = nullptr;
        } else {
            QMessageBox::warning(this,
                                 tr("Error"),       
                                 tr("Invalid input"));
            return;
        }
    }
}
