#include "include/rayib.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

const Color BG = Color{0, 0, 0, 255};

std::vector<std::string> songList = {"Synthesis"};
std::string chartID, chartName, artist;
int BPM = -1;
Music music;

void loadChart() {
  std::ifstream chart(chartID+".txt");

  if(!chart.is_open()) {
    std::cerr << "file to open the chart file\n"; 
    exit(0);
  }

  std::string line;
  
  while(std::getline(chart, line)) {
    if(!chartName.empty() && line.substr(0,4) == "NAME") {
      int pos = line.find(":");
      chartName = line.substr(pos + 1);
      continue;
    }

    if(!artist.empty() && line.substr(0,6) == "ARTIST") {
      int pos = line.find(":");
      artist = line.substr(pos + 1);
      continue;
    }

    if(BPM != -1 && line.substr(0,3) == "BPM") {
      int pos = line.find(":");
      BPM = stoi(line.substr(pos + 1));
      continue;
    }
  }

}

void songSelection() {
  int x = -1;
  for (int i=0 ; i<songList.size() ; i++) std::cout << i+1 << " " << songList[i] << std::endl;
  std::cout << "enter chart number:";
  // std::cin >> x;
  x -= 1;
  chartID = songList[0];
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
