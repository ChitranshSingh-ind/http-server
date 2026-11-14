#include <iostream>
int main() {
  std::cout << "Hello Whats Your Name? \n";
  std::string name;
  std::getline(std::cin, name);
  std::cout << "Hello " << name;
}
