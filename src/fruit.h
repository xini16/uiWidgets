#ifndef FRUIT_H
#define FRUIT_H
#include <string>

class Fruit {
public:
  Fruit(const std::string &name) : name(name) {}
  std::string getName() const { return name; }

private:
  std::string name;
};

#endif // FRUIT_H
