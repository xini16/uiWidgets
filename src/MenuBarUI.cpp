#include "MenuBarUI.h"

MenuBarUI::MenuBarUI(QWidget *parent) : QWidget(parent) {
  QHBoxLayout *layout = new QHBoxLayout(this);

  addButton = new QToolButton(this);
  addButton->setText("Add");
  addMenu = new QMenu(this);
  for (const auto &entry : resourceType.left) {
    ResourceType type = entry.first;
    std::string typeName = entry.second;
    QAction *action = addMenu->addAction(QString::fromStdString(typeName));

    connect(action, &QAction::triggered, this, [=]() {
      lastAddedType = type;
      if (selectedResource) {
        emit addResource(selectedResource.value(), "New Resource " + typeName,
                         type);
        return;
      }
      if (selectedInsertPoint) {
        ResourceTreeItem *parentItem = dynamic_cast<ResourceTreeItem *>(
            selectedInsertPoint.value()->parent());
        Resource *parent = parentItem->getResource();
        int index = parentItem->indexOfChild(selectedInsertPoint.value());
        emit insertNewResource(parent, "New Resource " + typeName, type, index);
        return;
      }
      assert(false);
    });
  }

  addButton->setMenu(addMenu);
  addButton->setPopupMode(QToolButton::MenuButtonPopup);
  connect(addButton, &QToolButton::clicked, this, [=]() {
    if (selectedResource) {
      emit addResource(selectedResource.value(), "New Resource ",
                       lastAddedType);
      return;
    }
    if (selectedInsertPoint) {
      ResourceTreeItem *parentItem = dynamic_cast<ResourceTreeItem *>(
          selectedInsertPoint.value()->parent());
      Resource *parent = parentItem->getResource();
      int index = parentItem->indexOfChild(selectedInsertPoint.value());
      emit insertNewResource(parent, "New Resource ", lastAddedType, index);
      return;
    }
    assert(false);
  });

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
  connect(sortButton, &QPushButton::clicked, this,
          [=]() { sortbuttonClicked(); });

  renameButton = new QPushButton("Rename", this);
  connect(renameButton, &QPushButton::clicked, this,
          &MenuBarUI::onRenameResource);

  deleteButton = new QPushButton("Delete", this);
  connect(deleteButton, &QPushButton::clicked, this,
          &MenuBarUI::onDeleteResource);

  layout->addWidget(addButton);
  layout->addWidget(sortButton);
  layout->addWidget(renameButton);
  layout->addWidget(deleteButton);
  setLayout(layout);
}

void MenuBarUI::sortbuttonClicked() {
  switch (order) {
  case None:
    order = Ascending;
    break;
  case Ascending:
    order = Descending;
    break;
  case Descending:
    order = None;
    break;
  default:
    assert(false);
  }
  std::string nextOrderText = sortOrder.left.at(order);
  sortButton->setText(QString::fromStdString(nextOrderText));
  emit sortResources(criteria, order);
}

void MenuBarUI::onDeleteResource() {
  if (selectedResource) {
    emit deleteResource(selectedResource.value());
    selectedResource = nullptr;
  }
}

void MenuBarUI::onRenameResource() {
  if (selectedResource) {
    bool inputFinished;
    QString newName = QInputDialog::getText(
        this, tr("Rename Resource"), tr("Enter new name for the resource:"),
        QLineEdit::Normal, "", &inputFinished);
    if (inputFinished && !newName.isEmpty()) {
      std::string stdNewName = newName.toStdString();
      emit renameResource(selectedResource.value(), stdNewName);
      selectedResource = nullptr;
    } else {
      QMessageBox::warning(this, tr("Error"), tr("Invalid input"));
      return;
    }
  }
}
