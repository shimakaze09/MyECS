//
// Created by Admin on 19/03/2025.
//

#include <MyTemplate/TypeID.h>

#include <iostream>

using namespace std;

int main(int argc, char** argv) {
  if (argc != 2)
    return 1;

  std::cout << My::TypeID(argv[1]).GetValue() << std::endl;

  return 0;
}
