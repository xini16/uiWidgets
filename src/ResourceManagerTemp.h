#ifndef RESOURCE_MANAGER_TEMP_H
#define RESOURCE_MANAGER_TEMP_H
class ResourceManager;
class Resource;
template <typename T> class ResourceManagerTemp {
public:
  ResourceManagerTemp<T>(ResourceManager *resourceManager);
  void createTestData();
  T *getItem(Resource *resource);

private:
  ResourceManager *resourceManager;
};

#endif