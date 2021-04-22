#include "Player.h"

bool Player::Moved() const
{
  if(coords.x == old_coords.x && coords.y == old_coords.y)
    return false;
  else
    return true;
}

void Player::change_curImage() {
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
            cur_image = 5;
            if (cur_frame >= 6) {
                t1 = glfwGetTime();
                cur_frame = 0;
            }
        }
    } else
    {
        if (cur_frame >= 0){
            cur_image = 6;
        }
        if (cur_frame >= 1){
            cur_image = 7;
        }
        if (cur_frame >= 2){
            cur_image = 8;
        }
        if (cur_frame >= 3){
            cur_image = 9;
        }
        if (cur_frame >= 4){
            cur_image = 10;
        }
        if (cur_frame >= 5){
            cur_image = 11;
            if (cur_frame >= 6) {
                t1 = glfwGetTime();
                cur_frame = 0;
            }
        }
    }
}



void Player::ProcessInput(MovementDir dir)
{
  int move_dist = move_speed * 1;
  switch(dir)
  {
    case MovementDir::UP:
        t2 = glfwGetTime();
        cur_frame += (t2 - t1)*fps;
        change_curImage();
        old_coords.y = coords.y;
        coords.y += move_dist;
        break;
    case MovementDir::DOWN:
        t2 = glfwGetTime();
        cur_frame += (t2 - t1)*fps;
        change_curImage();
        old_coords.y = coords.y;
        coords.y -= move_dist;
      break;
    case MovementDir::LEFT:
      if (direction != 3){
          direction = 3;
          cur_frame = 0;
          t1 = glfwGetTime();
      }

      t2 = glfwGetTime();
      cur_frame += (t2 - t1)*fps;
      change_curImage();

      old_coords.x = coords.x;
      coords.x -= move_dist;
      break;
    case MovementDir::RIGHT:
         if (direction != 4){
             direction = 4;
             cur_frame = 0;
             t1 = glfwGetTime();
        }

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

void Player::Draw(Image &screen, Image &bkg){


  if(Moved()){
    for(int y = old_coords.y; y <= old_coords.y + 31; ++y){
      for(int x = old_coords.x; x <= old_coords.x + 32; ++x)
      {
        screen.PutPixel(x, y, bkg.GetPixel(x, y));
      }
    }
    old_coords = coords;
  }

  for(int y = coords.y; y <= coords.y + 31; ++y)
  {
    for(int x = coords.x; x <= coords.x + 32; ++x)
    {
        Pixel current_pixel = anim[cur_image].GetPixel(x - coords.x  ,y - coords.y );
        screen.PutPixel(x, y, current_pixel.a >= 255 ? current_pixel : bkg.GetPixel(x,y));
    }

  }
} //Draw

void Player:: setStartpos(int x, int y){
    coords.x = x;
    coords.y = y;
    old_coords = coords;
}

bool Player:: escaped(Point escape){
    if (coords.x + 26 >= escape.x && coords.x + 8 <= escape.x && coords.y + 32 >= escape.y && coords.y + 6 <= escape.y) {
        return true;
    }
    return false;
}


void Player::colissionX(const std::vector<std::string> &lvl) {
    if (coords.x  >= (WINDOW_WIDTH - 30) || (coords.x )  < 0) {
        coords.x = old_coords.x;
        return;
    }
    for (int i=(old_coords.y+2)/32; i<(old_coords.y+63)/32;i++){
        for (int j=(coords.x+5)/32; j<(coords.x+59)/32;j++){
            if (lvl[i][j] == '#'){
                if (coords.x - old_coords.x != 0) {coords.x = old_coords.x;}
            }
        }
    }

}

void Player::colissionY(const std::vector<std::string> &lvl) {
    if (coords.y >= WINDOW_HEIGHT-30 || coords.y + 4  < 0) {
        coords.y = old_coords.y;
        return;
    }
    for (int i=(coords.y+2)/32; i<(coords.y+63)/32;i++){
        for (int j=(old_coords.x+5)/32; j<(old_coords.x+59)/32;j++){
            if (lvl[i][j] == '#'){
                if (coords.y - old_coords.y != 0) {coords.y = old_coords.y;}
            }
        }
    }

}

bool Player:: died(int actionmap[WINDOW_WIDTH/tileSize][WINDOW_HEIGHT/tileSize]){
    int x = (coords.x+15) / tileSize;
    int y = (coords.y+20)/ tileSize;

    if (actionmap[x][y] == 0){
        return true;
    }
    return false;
}


