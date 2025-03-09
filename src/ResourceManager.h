#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H
#include "Resource.h"
#include <QObject>


class ResourceManager : public QObject {
    Q_OBJECT

public:
    explicit ResourceManager(QObject *parent = nullptr);
    std::vector<Resource *> getRootResources() const;

public slots: 
    void addResource(Resource *parent, const std::string &name, const ResourceType type);
    void deleteResource(Resource *resource);
    void renameResource(Resource *resource, const std::string &newName);
    void sortResources(const std::string &criteria, const sortOrder &order);
    void insertChild(Resource *parent, Resource *child, std::size_t index);
    void removeChild(Resource *child);
    void createTestData();

signals:
    void resourceUpdated(); 

private:
    std::vector<Resource *> rootResources;
};

#endif // RESOURCEMANAGER_H
