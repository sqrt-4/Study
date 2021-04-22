#ifndef MAIN_ENEMY_H
#define MAIN_ENEMY_H

#include "Image.h"
#include "Player.h"



struct Enemy {
    Point coords {.x = 10, .y = 10};
    int cur_image = 0;
    //Image anim[10];
    Enemy(int x, int y, int type_):anim{
                  Image(enemyl0_path), Image(enemyl1_path),
                  Image(enemyl2_path), Image(enemyl3_path),
                  Image(enemyl4_path), Image(enemyr0_path),
                  Image(enemyr1_path), Image(enemyr2_path),
                  Image(enemyr3_path), Image(enemyr4_path)}
    {
        old_coords.x = x; old_coords.y = y;
        coords = old_coords; type = type_;
        direction = (type == 0) ? 3 : 0;
    };

    Enemy(const Enemy &enemy);

    void move(const std::vector<std::string> &lvl);
    void Draw(Image &screen, Image &bkg);
    bool kills(Player &player);
    bool colissionX (const std::vector<std::string> &lvl);
    bool colissionY (const std::vector<std::string> &lvl);
    void change_curImage();

private:
    Image anim[10];
    double t1 = 0.0, t2 = 0.0, cur_frame = 0;
    int direction = 0; //0 - up, 1 - down, 3 - left, 4 -right
    Point old_coords {.x = 10, .y = 10};
    int move_speed = 3;
    double fps = 2.5;
    int type; // 0 - horizontal, 1 - vertical
};


#endif //MAIN_ENEMY_H
