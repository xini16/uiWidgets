#include "MenuBarUI.h"
#include "ResourceManager.h"
#include "ResourceViewUI.h"
#include "fruit.h"
#include <QApplication>
#include <QVBoxLayout>
#include <QWidget>

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);

  ResourceManager<Fruit> resourceManager;
  // resourceManager.createTestData();

  QWidget mainWidget;
  QVBoxLayout *layout = new QVBoxLayout(&mainWidget);

  ResourceViewUI<Fruit> *resourceView =
      new ResourceViewUI<Fruit>(&resourceManager);
  MenuBarUI<Fruit> *menuBar = new MenuBarUI<Fruit>();
  QObject::connect(menuBar, &MenuBarUI<Fruit>::addResource, &resourceManager,
                   &ResourceManager<Fruit>::addResource);
  QObject::connect(menuBar, &MenuBarUI<Fruit>::renameResource, &resourceManager,
                   &ResourceManager<Fruit>::renameResource);
  QObject::connect(menuBar, &MenuBarUI<Fruit>::sortResources, resourceView,
                   &ResourceViewUI<Fruit>::sortResources);
  QObject::connect(menuBar, &MenuBarUI<Fruit>::deleteResource, &resourceManager,
                   &ResourceManager<Fruit>::deleteResource);
  QObject::connect(menuBar, &MenuBarUI<Fruit>::insertNewResource,
                   &resourceManager,
                   &ResourceManager<Fruit>::insertNewResource);
  QObject::connect(menuBar, &MenuBarUI<Fruit>::searchResource, resourceView,
                   &ResourceViewUI<Fruit>::filterResources);
  QObject::connect(resourceView, &ResourceViewUI<Fruit>::resourceSelected,
                   menuBar, [=](Resource<Fruit> *resource) {
                     menuBar->selectedResource = resource;
                     menuBar->selectedInsertPoint.reset();
                   });
  QObject::connect(resourceView, &ResourceViewUI<Fruit>::insertPointSelected,
                   menuBar, [=](QTreeWidgetItem *insertPoint) {
                     menuBar->selectedInsertPoint = insertPoint;
                     menuBar->selectedResource.reset();
                   });
  layout->addWidget(menuBar);
  layout->addWidget(resourceView);

  mainWidget.show();
  return app.exec();
}
