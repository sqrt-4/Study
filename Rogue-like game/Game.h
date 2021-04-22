#ifndef MAIN_GAME_H
#define MAIN_GAME_H

#include <vector>
#include <string>
#include "Image.h"
#include "Player.h"
#include <GLFW/glfw3.h>
#include "Enemy.h"


class Game {
    Image map;
    Point prev_pos = {.x=0, .y =0};
    double t1_exitAnim = 0, t2_exitAnim = 0;
    int cur_exitImg = 0;
    double cur_exitFrame = 0;

public:
    std::vector<std::string> lvl;
    int actionmap [WINDOW_WIDTH/tileSize][WINDOW_HEIGHT/tileSize];
    Point escape;

    Image floor;
    Image wall;
    Image empty;
    Image ruin0;
    Image ruin1;
    Image ruin2;
    Image ruin3;
    Image playerl0;
    Image playerl1;
    Image playerl2;
    Image playerl3;
    Image playerl4;
    Image playerl5;
    Image playerr0;
    Image playerr1;
    Image playerr2;
    Image playerr3;
    Image playerr4;
    Image playerr5;
    Image anim_exit[4];

    Game(): map(Image(WINDOW_WIDTH, WINDOW_HEIGHT, 4)),
            floor(Image(floorImg_path)), wall(Image(wallImg_path)),
            empty(Image(emptyImg_path)), ruin0(Image(ruin0Img_path)),
            ruin1(Image(ruin1Img_path)), ruin2(Image(ruin2Img_path)),
            ruin3(Image(ruin3Img_path)),
            anim_exit{Image(exit0Img_path), Image(exit1Img_path),
                      Image(exit2Img_path), Image(exit3Img_path)},
            playerl0(Image(playerl0_path)), playerl1(Image(playerl1_path)),
            playerl2(Image(playerl2_path)), playerl3(Image(playerl3_path)),
            playerl4(Image(playerl4_path)), playerl5(Image(playerl5_path)),
            playerr0(Image(playerr0_path)), playerr1(Image(playerr1_path)),
            playerr2(Image(playerr2_path)), playerr3(Image(playerr3_path)),
            playerr4(Image(playerr4_path)), playerr5(Image(playerr5_path)) {};




    void set_lvl(std::string path);
    void draw_lvl(Image &screen, Player &player);
    void printTXT_map();
    void print_actionmap();
    void update(Image &map,Image &screen, Player &player);
    void mozaik(Image image_before, Image image_after, GLFWwindow*  window, int after);

    std::vector<Enemy> enemies;
};


#endif //MAIN_GAME_H
