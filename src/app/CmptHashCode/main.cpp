//
// Created by Admin on 19/03/2025.
//

#include <MyECS/CmptType.h>
 
 #include <iostream>
 
 using namespace std;
 
int main(int argc, char** argv) {
  if (argc != 2)
    return 1;
 
  std::cout << My::MyECS::CmptType(argv[1]).HashCode() << std::endl;
 
  return 0;
}
