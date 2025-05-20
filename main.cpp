#include "include/rayib.h"
#include <iostream>
#include <fstream>
#include <ostream>
#include <string>
#include <sstream>
#include <vector>

using std::string;using std::vector;using std::cout;

const Color BG = Color{0, 0, 0, 255};
const string song_folder = "songs/";
const int screenHeight = 720;
const int screenWidth = 1280;


string selected_song;
vector<std::string> songList = {"Synthesis"};

float chartTime_curr;
float chartLength;
bool pause = false;

float scrollSpeed = 1000;
int note_thickness = 100;
int note_height = 50;

class Note {
  public:
    int type;
    int lane;
    float time;
    float hold_duration = 0;
    Note(int type, int lane, float time, float hold_duration) {
      this->type = type;
      this->lane = lane;
      this->time = time * 0.001;
      this->hold_duration = hold_duration * 0.001;
    }
    void display() {
      cout << "type: " << type<< ' ';
      cout << "lane: " << lane<< ' ';
      cout << "time: " << time << ' ';
      cout << "hold: " << hold_duration << '\n';
    }
};

class chartData {
  public:
    string name, artist;
    double BPM = -1;
    Music music;
    float offset = 0;
    vector<Note> notes;

    void insertNoteData(string line) {
      string type, lane, time, hold_duration = "0";
      std::istringstream ss(line);
      std::getline(ss, type, ':');
      std::getline(ss, lane, ':');
      std::getline(ss, time, ':');
      std::getline(ss, hold_duration, ':');

      notes.push_back(Note(stoi(type), stoi(lane), stof(time)+offset, stof(hold_duration)));
    }

    Music loadMusic(string song) {
      song = song_folder + selected_song + '/' + song;
      const char* audioFile = song.c_str();
      Music music = LoadMusicStream(audioFile);

      if (music.stream.buffer == NULL || music.stream.sampleRate == 0) {
        std::cerr << "Failed to load music \n";
        exit(0);
      }

      chartLength = GetMusicTimeLength(music);

      return music;
    }

    void loadChartData() {
      std::ifstream file(song_folder + selected_song + "/data.vsr");

      if(!file.is_open()) {
        std::cerr << "can't open " + songList[0] << '\n';
        exit(0);
      }
      const string s;
      string line;
      while(std::getline(file,line)) {
        if(line.find("#") == 0) insertNoteData(line.substr(1));
        else if(line.find("[TITLE]") == 0) name = line.substr(8);
        else if(line.find("[ARTIST]") == 0) artist = line.substr(9);
        else if(line.find("[BPM]") == 0) BPM = stod(line.substr(6));
        else if(line.find("[OFFSET]") == 0) offset = stof(line.substr(9));
        else if(line.find("[SONG]") == 0) music = loadMusic(line.substr(7));
      }

    }

};


void getInput(chartData& chart) {
  if (IsKeyPressed(KEY_SPACE))
  {
    pause = !pause;
    if(pause) PauseMusicStream(chart.music);
    else ResumeMusicStream(chart.music);
  }
}

class Game{
  public:
    void update(chartData& chart) {
      chartTime_curr = GetMusicTimePlayed(chart.music);
      updateNotes(chart, chartTime_curr);
      draw(chart, std::to_string(chartTime_curr));
    }

    void draw(chartData chart, string chartTime) {
      string length = std::to_string(chartLength);
      chartTime += " / " + length; 
      const char* timePlayed = chartTime.c_str();
      DrawText(timePlayed, 10, 10, 20, WHITE);
    }

    void updateNotes(chartData& chart,float chartTime) {
      for (Note& note : chart.notes) {
        float noteY = screenHeight - (note.time - chartTime) * scrollSpeed - int(note_height/2);
        cout << noteY << ' ';
        if(noteY > screenHeight || noteY < 0) continue;
        int noteX = screenWidth/2 + (note.lane-2)*note_thickness;
        DrawRectangle(noteX, noteY, note_thickness, note_height, WHITE);
      }
      cout << std::endl;
    }
};


int main() {
  InitWindow(screenWidth, screenHeight, "VSRG");
  InitAudioDevice();
  SetTargetFPS(60);

  selected_song = songList[0];

  Game game;
  chartData chart;

  chart.loadChartData();
  cout << "name: "<< chart.name << "\nartist: " << chart.artist << "\nBPM: " << chart.BPM << '\n';

  PlayMusicStream(chart.music);
  pause = true;
  PauseMusicStream(chart.music);
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
