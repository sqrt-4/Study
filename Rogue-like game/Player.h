#ifndef MAIN_PLAYER_H
#define MAIN_PLAYER_H

#include "Image.h"
#include <string>
#include <vector>
#include <GLFW/glfw3.h>

struct Point
{
  int x;
  int y;
};

enum class MovementDir
{
  UP,
  DOWN,
  LEFT,
  RIGHT
};

struct Player
{
  explicit Player(Point pos = {.x = 10, .y = 10}) :
                 coords(pos), old_coords(coords),
                 anim{Image(playerl0_path), Image(playerl1_path),
                      Image(playerl2_path), Image(playerl3_path),
                      Image(playerl4_path), Image(playerl5_path),
                      Image(playerr0_path), Image(playerr1_path),
                      Image(playerr2_path), Image(playerr3_path),
                      Image(playerr4_path), Image(playerr5_path)} {};

  bool Moved() const;
  void ProcessInput(MovementDir dir);
  void Draw(Image &screen, Image &bkg);
  void setStartpos(int x, int y);
  void colissionX (const std::vector<std::string> &lvl);
  void colissionY (const std::vector<std::string> &lvl);
  bool escaped(Point escape);
  bool died(int actionmap[WINDOW_WIDTH/tileSize][WINDOW_HEIGHT/tileSize]);

  Point coords {.x = 10, .y = 10};
  int cur_image = 6;
  Image anim[12];
private:
  double t1 = 0.0, t2 = 0.0, cur_frame = 0;
  int direction = 0; //0 - up, 1 - down, 3 - left, 4 -right
  Point old_coords {.x = 10, .y = 10};
  Pixel color {.r = 255, .g = 255, .b = 0, .a = 255};
  int move_speed = 3;
  double fps = 2.5;

  void change_curImage();

};

#endif //MAIN_PLAYER_H
