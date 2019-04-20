//
// Created by Jan on 19.04.2019.
//
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <chrono>

void split(const std::string& s, char c,
           std::vector<std::string>& v) {
  std::string::size_type i = 0;
  std::string::size_type j = s.find(c);

  while (j != std::string::npos) {
    v.push_back(s.substr(i, j-i));
    i = ++j;
    j = s.find(c, j);

    if (j == std::string::npos)
      v.push_back(s.substr(i, s.length()));
  }
}

struct Vector3f{
  float x,y,z;
  Vector3f(float x, float y, float z) : x(x), y(y), z(z) {}
};

int main(){
  auto begin = std::chrono::steady_clock::now();
  std::ifstream file(R"(M:\Projekte\2019\recap_test\Aero_Scan.pts)");
  std::vector<char> str;
  std::vector<float> floats;
  /*std::getline(file, str);

  std::vector<std::string> test;
  while (std::getline(file, str))
  {
    test.clear();
    split(str, ' ', test);
    positions.emplace_back(std::stof(test[0]), std::stof(test[1]), std::stof(test[2]));
  }
  std::cout << positions.size() << std::endl;*/
  file.seekg(0, std::ios::end);
  str.resize(file.tellg());
  file.seekg(0);
  file.read(str.data(), str.size());
  std::string bufferString;

  std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
  auto microseconds = std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();
  std::cout << microseconds *0.0000006 << std::endl;
  std::cout << floats.size() / 3 << std::endl;
  return 0;
}