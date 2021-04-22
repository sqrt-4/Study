//
// Created by sqrt4 on 03.03.2021.
//

#include "Enemy.h"

Enemy:: Enemy(const Enemy &enemy):anim{
                Image(enemyl0_path), Image(enemyl1_path),
                Image(enemyl2_path), Image(enemyl3_path),
                Image(enemyl4_path), Image(enemyr0_path),
                Image(enemyr1_path), Image(enemyr2_path),
                Image(enemyr3_path), Image(enemyr4_path)},
                                  coords(enemy.coords), old_coords(enemy.old_coords),
                                  type(enemy.type), move_speed(enemy.move_speed),
                                  direction(enemy.direction), fps(enemy.fps),
                                  t1(enemy.t1), t2(enemy.t2), cur_frame(enemy.cur_frame)
{};

void Enemy:: move(const std::vector<std::string> &lvl){
    if (type == 0 && colissionX(lvl)){
        direction = (direction == 3) ? 4 : 3;
    }
    if (type == 1 && colissionY(lvl)){
        direction = (direction == 0) ? 1 : 0;
    }
    int move_dist = move_speed*1;
    switch(direction){
        case (0):
            t2 = glfwGetTime();
            cur_frame += (t2 - t1)*fps;
            change_curImage();
            old_coords.y = coords.y;
            coords.y += move_dist;
            break;
        case (1):
            t2 = glfwGetTime();
            cur_frame += (t2 - t1)*fps;
            change_curImage();
            old_coords.y = coords.y;
            coords.y -= move_dist;
            break;
        case (3):
            t2 = glfwGetTime();
            cur_frame += (t2 - t1)*fps;
            change_curImage();
            old_coords.x = coords.x;
            coords.x -= move_dist;
            break;
        case (4):
            t2 = glfwGetTime();
            cur_frame += (t2 - t1)*fps;
            change_curImage();
            old_coords.x = coords.x;
            coords.x += move_dist;
            break;
        default:
            break;
    }
}

bool Enemy::colissionX(const std::vector<std::string> &lvl) {
    if (coords.x  >= (WINDOW_WIDTH - 32) || (coords.x ) < 0) {
        coords.x = old_coords.x;
        return true;
    }
    for (int i=(coords.y+2)/32; i<(coords.y+62)/32;i++){         /////////0..64
        for (int j=(coords.x+2)/32; j<(coords.x+62)/32;j++){    /////////0..64
            if (lvl[i][j] == '#'){
                    coords.x = old_coords.x;
                    return true;
            }
        }
    }
    return false;
}

bool Enemy::colissionY(const std::vector<std::string> &lvl) {
    if (coords.y >= WINDOW_HEIGHT-32) {
        coords.y = old_coords.y;
        return true;
    }
    for (int i=(coords.y+2)/32; i<(coords.y+62)/32;i++){         /////////0..64
        for (int j=(coords.x+2)/32; j<(coords.x+62)/32;j++){    /////////0..64
            if (lvl[i][j] == '#'){
                    coords.y = old_coords.y;
                    return true;
            }
        }
    }
    return false;
}

void Enemy::Draw(Image &screen, Image &bkg) {
    for (int y = old_coords.y; y <= old_coords.y + 31; ++y) {
        for (int x = old_coords.x; x <= old_coords.x + 32; ++x) {
            screen.PutPixel(x, y, bkg.GetPixel(x, y));
        }
    }
    old_coords = coords;
    for (int y = coords.y; y <= coords.y + 31; ++y) {
        for (int x = coords.x; x <= coords.x + 32; ++x) {
            Pixel current_pixel = anim[cur_image].GetPixel(x - coords.x, y - coords.y);
            screen.PutPixel(x, y, current_pixel.a >= 255 ? current_pixel : bkg.GetPixel(x, y));
        }
    }
}

bool Enemy:: kills(Player &player){
    for (int x = (player.coords.x)/tileSize; x < (player.coords.x + 64)/tileSize; x++){
        for (int y = (player.coords.y)/tileSize; y < (player.coords.y+64)/tileSize; y++){
            if (x == coords.x/tileSize && y == coords.y/tileSize){
                return true;
            }
        }
    }
    return false;
}

void Enemy::change_curImage() {
    if (direction == 3) {
        if (cur_frame >= 0) {
            cur_image = 0;
        }
        if (cur_frame >= 1) {
            cur_image = 1;
        }
        if (cur_frame >= 2) {
            cur_image = 2;
        }
        if (cur_frame >= 3) {
            cur_image = 3;
        }
        if (cur_frame >= 4) {
            cur_image = 4;
        }
        if (cur_frame >= 5) {
            cur_image = 3;
            if (cur_frame >= 6) {
                t1 = glfwGetTime();
                cur_frame = 0;
            }
        }
    } else
    {
        if (cur_frame >= 0){
            cur_image = 5;
        }
        if (cur_frame >= 1){
            cur_image = 6;
        }
        if (cur_frame >= 2){
            cur_image = 7;
        }
        if (cur_frame >= 3){
            cur_image = 8;
        }
        if (cur_frame >= 4){
            cur_image = 9;
        }
        if (cur_frame >= 5){
            cur_image = 7;
            if (cur_frame >= 6) {
                t1 = glfwGetTime();
                cur_frame = 0;
            }
        }
    }
}