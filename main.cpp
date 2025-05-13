#include "include/rayib.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

const Color BG = Color{0, 0, 0, 255};

std::string chartName;
Music music;
std::vector<std::string> songList = {"Synthesis"};

void loadChart() {
  std::ifstream chart(chartName);

  if(!chart.is_open()) {
    std::cerr << "file to open the chart file\n"; 
    exit(0);
  }

  std::string line;
  
  while(std::getline(file, line)) {

  }

}

void songSelection() {
  int x = -1;
  for (int i=0 ; i<songList.size() ; i++) std::cout << i+1 << " " << songList[i] << std::endl;
  std::cout << "enter chart number:";
  // std::cin >> x;
  x -= 1;
  chartName = songList[0];
}

int main() {
  InitWindow(1280, 720, "VSRG");
  SetTargetFPS(60);

  songSelection();
  loadChart();

  while(!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(BG);
    EndDrawing();
  }
  CloseWindow();
  return 0;
}
