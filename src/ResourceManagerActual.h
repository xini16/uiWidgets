#ifndef RESOURCE_MANAGER_ACTUAL_H
#define RESOURCE_MANAGER_ACTUAL_H
class ResourceManager;
class Resource;
template <typename T> class ResourceManagerActual {
public:
  ResourceManagerActual<T>(ResourceManager *resourceManager);
  void createTestData();
  T *getItem(Resource *resource);

private:
  ResourceManager *resourceManager;
};

#endif