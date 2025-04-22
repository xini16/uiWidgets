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

  layout->addWidget(menuBar);
  layout->addWidget(resourceView);

  mainWidget.show();
  return app.exec();
}
