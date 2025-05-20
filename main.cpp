#include "include/rayib.h"
#include <iostream>
#include <fstream>
#include <iterator>
#include <ostream>
#include <string>
#include <vector>

using std::string;using std::vector;using std::cout;

const Color BG = Color{0, 0, 0, 255};
const string song_folder = "songs/";

string selected_song;
vector<std::string> songList = {"Synthesis"};

string chartTime_curr;
string chartLength;
bool pause = false;

class chartData {
  public:
    string name, artist;
    double BPM = -1;
    Music music;

    Music loadMusic(string song) {
      song = song_folder + selected_song + '/' + song;
      const char* audioFile = song.c_str();
      Music music = LoadMusicStream(audioFile);

      if (music.stream.buffer == NULL || music.stream.sampleRate == 0) {
        std::cerr << "Failed to load music: \n";
        exit(0);
      }

      chartLength = std::to_string(GetMusicTimeLength(music));

      return music;
    }

    chartData loadChartData() {
      chartData data;
      std::ifstream file(song_folder + selected_song + "/data.txt");

      if(!file.is_open()) {
        std::cerr << "can't open " + songList[0] << '\n';
        exit(0);
      }
      const string s;
      string line;
      while(std::getline(file,line)) {
        if(line.find("#TITLE") == 0) data.name = line.substr(7);
        if(line.find("#ARTIST") == 0) data.artist = line.substr(8);
        if(line.find("#BPM") == 0) data.BPM = stod(line.substr(5));
        if(line.find("#SONG") == 0) data.music = loadMusic(line.substr(6));
      }

      return data;
    }
};


void getInput(chartData& chart) {
  if (IsKeyPressed(KEY_SPACE))
  {
    StopMusicStream(chart.music);
    PlayMusicStream(chart.music);
  }

  if (IsKeyPressed(KEY_P))
  {
    pause = !pause;
    if(pause) PauseMusicStream(chart.music);
    else ResumeMusicStream(chart.music);
  }
}

class Game{
  public:
    void update(chartData& chart) {
      chartTime_curr = std::to_string(GetMusicTimePlayed(chart.music));

      draw(chart, chartTime_curr);
    }

    void draw(chartData chart, string chartTime) {
      chartTime += " / " + chartLength; 
      const char* timePlayed = chartTime.c_str();
      DrawText(timePlayed, 10, 10, 20, WHITE);
    }
};


int main() {
  InitWindow(1280, 720, "VSRG");
  InitAudioDevice();
  SetTargetFPS(60);

  selected_song = songList[0];

  Game game;
  chartData chart;

  chart = chart.loadChartData();
  cout << "name: "<< chart.name << "\nartist: " << chart.artist << "\nBPM: " << chart.BPM << '\n';
  

  while(!WindowShouldClose()) {
    BeginDrawing();
    UpdateMusicStream(chart.music);
    getInput(chart);
    game.update(chart);
    ClearBackground(BG);
    EndDrawing();
  }
  UnloadMusicStream(chart.music);
  CloseAudioDevice(); 
  CloseWindow();
  return 0;
}
