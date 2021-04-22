#ifndef MAIN_IMAGE_H
#define MAIN_IMAGE_H

#include <string>
#include "common.h"

constexpr int WINDOW_WIDTH = 1600;
constexpr int WINDOW_HEIGHT = 1024;
constexpr int tileSize = 32;


const std:: string playerl0_path = "../Resources/Images/Player/player_left0.png";
const std:: string playerl1_path = "../Resources/Images/Player/player_left1.png";
const std:: string playerl2_path = "../Resources/Images/Player/player_left2.png";
const std:: string playerl3_path = "../Resources/Images/Player/player_left3.png";
const std:: string playerl4_path = "../Resources/Images/Player/player_left4.png";
const std:: string playerl5_path = "../Resources/Images/Player/player_left5.png";

const std:: string playerr0_path = "../Resources/Images/Player/player_right0.png";
const std:: string playerr1_path = "../Resources/Images/Player/player_right1.png";
const std:: string playerr2_path = "../Resources/Images/Player/player_right2.png";
const std:: string playerr3_path = "../Resources/Images/Player/player_right3.png";
const std:: string playerr4_path = "../Resources/Images/Player/player_right4.png";
const std:: string playerr5_path = "../Resources/Images/Player/player_right5.png";

const std:: string enemyl0_path = "../Resources/Images/Enemy/enemy_l0.png";
const std:: string enemyl1_path = "../Resources/Images/Enemy/enemy_l1.png";
const std:: string enemyl2_path = "../Resources/Images/Enemy/enemy_l2.png";
const std:: string enemyl3_path = "../Resources/Images/Enemy/enemy_l3.png";
const std:: string enemyl4_path = "../Resources/Images/Enemy/enemy_l4.png";

const std:: string enemyr0_path = "../Resources/Images/Enemy/enemy_r0.png";
const std:: string enemyr1_path = "../Resources/Images/Enemy/enemy_r1.png";
const std:: string enemyr2_path = "../Resources/Images/Enemy/enemy_r2.png";
const std:: string enemyr3_path = "../Resources/Images/Enemy/enemy_r3.png";
const std:: string enemyr4_path = "../Resources/Images/Enemy/enemy_r4.png";

const std:: string floorImg_path = "../Resources/Images/floor.png";
const std:: string wallImg_path = "../Resources/Images/wall.png";
const std:: string emptyImg_path = "../Resources/Images/empty.png";

const std:: string exit0Img_path = "../Resources/Images/exit0.png";
const std:: string exit1Img_path = "../Resources/Images/exit1.png";
const std:: string exit2Img_path = "../Resources/Images/exit2.png";
const std:: string exit3Img_path = "../Resources/Images/exit3.png";

const std:: string ruin0Img_path = "../Resources/Images/ruin0.png";
const std:: string ruin1Img_path = "../Resources/Images/ruin1.png";
const std:: string ruin2Img_path = "../Resources/Images/ruin2.png";
const std:: string ruin3Img_path = "../Resources/Images/ruin3.png";
const std:: string lvl2Img_path = "../Resources/Images/loading_lvl2.png";
const std:: string gameoverImg_path = "../Resources/Images/game_over.png";
const std:: string gamestartImg_path = "../Resources/Images/game_start.png";
const std:: string diedImg_path = "../Resources/Images/died.png";
const std:: string mapImg_path = "../Resources/Levels/map.png";


struct Pixel
{
  uint8_t r;
  uint8_t g;
  uint8_t b;
  uint8_t a;
};

constexpr Pixel backgroundColor{0, 0, 0, 0};

struct Image
{

  std:: string path;
  explicit Image(const std::string &a_path);
  Image(int a_width, int a_height, int a_channels);

  int Save(const std::string &a_path);

  int Width()    const { return width; }
  int Height()   const { return height; }
  int Channels() const { return channels; }
  size_t Size()  const { return size; }
  Pixel* Data()        { return  data; }

  Pixel GetPixel(int x, int y) { return data[width * y + x];}
  void  PutPixel(int x, int y, const Pixel &pix) { data[width * y + x] = pix; }
  void drawTile(Image &image,int x, int y);
  void set_image(const std::string &a_path);


  ~Image();

private:
  int width = -1;
  int height = -1;
  int channels = 3;
  size_t size = 0;
  Pixel *data = nullptr;
  bool self_allocated = false;

};



#endif //MAIN_IMAGE_H
