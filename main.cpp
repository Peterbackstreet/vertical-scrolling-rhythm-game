#include "include/rayib.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using std::string;using std::vector;using std::cout;

const Color BG = Color{0, 0, 0, 255};

vector<std::string> songList = {"Synthesis.txt"};

class chartData {
  public:
    string name, artist;
    int BPM = -1;
    Music music;
};


chartData loadChartData() {
  chartData data;
  std::ifstream file(songList[0]);

  if(!file.is_open()) {
    std::cerr << "can't open " + songList[0] << '\n';
    exit(0);
  }

  string line;
  while(std::getline(file,line)) {
    if(line.find("#TITLE") == 0) data.name = line.substr(7);
    if(line.find("#ARTIST") == 0) data.artist = line.substr(8);
    if(line.find("#BPM") == 0) data.BPM = stoi(line.substr(5));
  }

  return data;
}

int main() {
  InitWindow(1280, 720, "VSRG");
  SetTargetFPS(60);

  chartData chart;

  chart = loadChartData();
  cout << "name: "<< chart.name << "\nartist: " << chart.artist << "\nBPM: " << chart.BPM << '\n';

  while(!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(BG);
    EndDrawing();
  }
  CloseWindow();
  return 0;
}
