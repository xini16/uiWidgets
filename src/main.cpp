#include "MenuBarUI.h"
#include "ResourceManager.h"
#include "ResourceManagerActual.h"
#include "ResourceViewUI.h"
#include "fruit.h"
#include "src/ResourceList.h"
#include "src/fruitExample.h"
#include <QApplication>
#include <QVBoxLayout>
#include <QWidget>
#include <iostream>

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);

  ResourceManager resourceManager;
  ResourceManagerActual<Fruit> resourceManagerActual(&resourceManager);
  resourceManagerActual.createTestData();

  QWidget mainWidget;
  QVBoxLayout *layout = new QVBoxLayout(&mainWidget);

  ResourceViewUI *resourceView =
      new ResourceViewUI(&resourceManager, fruitFactoryMap);
  MenuBarUI *menuBar =
      new MenuBarUI(&resourceManager, resourceView, fruitFactoryMap);
  QObject::connect(menuBar, &MenuBarUI::addResource, &resourceManager,
                   &ResourceManager::addResource);
  QObject::connect(menuBar, &MenuBarUI::renameResource, &resourceManager,
                   &ResourceManager::renameResource);
  QObject::connect(menuBar, &MenuBarUI::sortResources, resourceView,
                   &ResourceViewUI::sortResources);
  QObject::connect(menuBar, &MenuBarUI::deleteResource, &resourceManager,
                   &ResourceManager::deleteResource);
  QObject::connect(menuBar, &MenuBarUI::insertNewResource, &resourceManager,
                   &ResourceManager::insertNewResource);
  QObject::connect(menuBar, &MenuBarUI::searchResource, resourceView,
                   &ResourceViewUI::filterResources);

  layout->addWidget(menuBar);
  layout->addWidget(resourceView);

  mainWidget.show();
  return app.exec();
}
