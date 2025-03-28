#ifndef RESOURCE_MANAGER_TEMP_H
#define RESOURCE_MANAGER_TEMP_H
class ResourceManager;
template <typename T> class ResourceManagerTemp {
public:
  ResourceManagerTemp<T>(ResourceManager *resourceManager);
  void createTestData();

private:
  ResourceManager *resourceManager;
};

#endif