#include "MenuBarUI.h"
#include "ResourceManager.h"
#include "ResourceViewUI.h"
#include <QApplication>
#include <QVBoxLayout>
#include <QWidget>

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);

  ResourceManager resourceManager;
  resourceManager.createTestData();

  QWidget mainWidget;
  QVBoxLayout *layout = new QVBoxLayout(&mainWidget);

  ResourceViewUI *resourceView = new ResourceViewUI(&resourceManager);
  MenuBarUI *menuBar = new MenuBarUI();

  QObject::connect(menuBar, &MenuBarUI::addResource, &resourceManager,
                   &ResourceManager::addResource);
  QObject::connect(menuBar, &MenuBarUI::renameResource, &resourceManager,
                   &ResourceManager::renameResource);
  QObject::connect(menuBar, &MenuBarUI::sortResources, &resourceManager,
                   &ResourceManager::sortResources);
  QObject::connect(menuBar, &MenuBarUI::deleteResource, &resourceManager,
                   &ResourceManager::deleteResource);
  QObject::connect(
      resourceView, &ResourceViewUI::resourceSelected, menuBar,
      [=](Resource *resource) { menuBar->selectedResource = resource; });

  layout->addWidget(menuBar);
  layout->addWidget(resourceView);

  mainWidget.show();
  return app.exec();
}
