#include "Game.h"
#include "Image.h"
#include <iostream>
#include <fstream>
#include "Enemy.h"


void Game:: set_lvl(std::string path){
    std::ifstream lvl_file;
    lvl_file.open(path);
    if (!lvl_file){
        std::cerr<<"Can't open: "<<path<<std::endl;
    }
    lvl.clear();
    while (!lvl_file.eof()){
        std::string str_tmp;
        getline(lvl_file, str_tmp);
        lvl.push_back(str_tmp);
    }
} //set_lvl

void Game::draw_lvl(Image &screen,Player &player) {
    for (int y=0; y<lvl.size(); y++){
        for (int x=0; x<lvl[y].size(); x++){
            switch (lvl[y][x]){
                case '.' : {
                             screen.drawTile(floor,x*tileSize,y*tileSize);
                             actionmap[x][y] = -5;
                             break;
                           }
                case '#' : {
                            screen.drawTile(wall,x*tileSize,y*tileSize);
                            actionmap[x][y] = -6;
                            break;
                           }
                case ' ' : {
                            screen.drawTile(empty,x*tileSize,y*tileSize);
                            actionmap[x][y] = 0;
                            break;
                           }
                case 'x' : {
                            screen.drawTile(anim_exit[0], x*tileSize, y*tileSize);
                            escape.x = x*tileSize+tileSize/2;
                            escape.y = y*tileSize+tileSize/2;
                            actionmap[x][y] = -8;
                            break;
                           }
                case ',' : {
                            screen.drawTile(ruin0, x*tileSize, y*tileSize);
                            actionmap[x][y] = 3;
                            break;
                           }
                case '@' : {
                            player.setStartpos(x*tileSize, y*tileSize);
                            screen.drawTile(floor,x*tileSize,y*tileSize);
                            actionmap[x][y] = -5;
                            break;
                           }
                case 'h' : {
                            Enemy enemy(x*tileSize, y*tileSize, 0);
                            enemies.push_back(enemy);
                            screen.drawTile(floor, x*tileSize, y*tileSize);
                            actionmap[x][y] = -5;
                            break;
                            }
                case 'v' : {
                    Enemy enemy(x*tileSize, y*tileSize, 1);
                    enemies.push_back(enemy);
                    screen.drawTile(floor, x*tileSize, y*tileSize);
                    actionmap[x][y] = -5;
                    break;
                }
            }
        }
    }
}//draw_lvl

void Game::update(Image &map,Image &screen,Player &player){

    int x = (player.coords.x+15) / tileSize;
    int y = (player.coords.y+15)/ tileSize;
    if (actionmap[x][y] > 0 && actionmap[x][y] < 4){
        switch(actionmap[x][y]){
            case 3: {
                map.drawTile(ruin2,x*tileSize,y*tileSize);
                screen.drawTile(ruin2,x*tileSize,y*tileSize);
                //player.Draw(screen,map);
                break;
            }
            case 2: {
                map.drawTile(ruin2,x*tileSize,y*tileSize);
               screen.drawTile(ruin2,x*tileSize,y*tileSize);
                //player.Draw(screen,map);
                break;
            }
            case 1: {
                map.drawTile(empty,x*tileSize,y*tileSize);
                screen.drawTile(empty,x*tileSize,y*tileSize);
                //player.Draw(screen,map);
                break;
            }
        }
       if (prev_pos.x !=x || prev_pos.y != y){
           actionmap[x][y]--;
       }
    }
    prev_pos.x=x;
    prev_pos.y=y;

    t2_exitAnim = glfwGetTime();
    cur_exitFrame += (t2_exitAnim - t1_exitAnim)*8;

    if (cur_exitFrame >= 0){
        cur_exitImg = 0;
        t1_exitAnim = glfwGetTime();
    }
    if (cur_exitFrame >= 70){
        cur_exitImg = 1;
    }
    if (cur_exitFrame >= 71){
        cur_exitImg = 2;
    }
    if (cur_exitFrame >= 72){
        cur_exitImg = 3;
    }
    if (cur_exitFrame >= 92){
        cur_exitImg = 2;
    }
    if (cur_exitFrame >= 93){
        cur_exitImg = 1;
    }
    if (cur_exitFrame >= 94){
        t1_exitAnim = glfwGetTime();
        cur_exitFrame = 0;
        cur_exitImg = 0;
    }
    screen.drawTile(anim_exit[cur_exitImg], escape.x - tileSize/2 , escape.y - tileSize/2);

}

void Game:: mozaik(Image image_before, Image image_after, GLFWwindow*  window, int after){
    if (after == 0) {
        for (int y = 0; y < WINDOW_HEIGHT; y++) {
            for (int x = 0; x < WINDOW_WIDTH; x++) {
                if ((y / 32) % 2 == 0 && (x / 32) % 2 == 0) {
                    image_before.PutPixel(x, y, Pixel{.r = image_after.GetPixel(x, y).r, .g = image_after.GetPixel(x, y).g,
                            .b = image_after.GetPixel(x, y).b, .a = image_after.GetPixel(x, y).a});
                } else if ((y / 32) % 2 == 1 && (x / 32) % 2 == 1) {
                    image_before.PutPixel(x, y, Pixel{.r = image_after.GetPixel(x, y).r, .g = image_after.GetPixel(x, y).g,
                            .b = image_after.GetPixel(x, y).b, .a = image_after.GetPixel(x, y).a});
                }
            }

            if (y % tileSize == 0) {
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                GL_CHECK_ERRORS;
                glDrawPixels(WINDOW_WIDTH, WINDOW_HEIGHT, GL_RGBA, GL_UNSIGNED_BYTE, image_before.Data());
                GL_CHECK_ERRORS;
                glfwSwapBuffers(window);
            }
        }

        for (int x = 0; x < WINDOW_WIDTH; x++) {
            for (int y = 0; y < WINDOW_HEIGHT; y++) {
                if ((y / 32) % 2 == 0 && (x / 32) % 2 == 1) {
                    image_before.PutPixel(x, y, Pixel{.r = image_after.GetPixel(x, y).r, .g = image_after.GetPixel(x, y).g,
                            .b = image_after.GetPixel(x, y).b, .a = image_after.GetPixel(x, y).a});
                } else if ((y / 32) % 2 == 1 && (x / 32) % 2 == 0) {
                    image_before.PutPixel(x, y, Pixel{.r = image_after.GetPixel(x, y).r, .g = image_after.GetPixel(x, y).g,
                            .b = image_after.GetPixel(x, y).b, .a = image_after.GetPixel(x, y).a});
                }
            }
            if (x % tileSize == 0) {
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                GL_CHECK_ERRORS;
                glDrawPixels(WINDOW_WIDTH, WINDOW_HEIGHT, GL_RGBA, GL_UNSIGNED_BYTE, image_before.Data());
                GL_CHECK_ERRORS;
                glfwSwapBuffers(window);
            }
        }
    } else{
        for (int y = WINDOW_HEIGHT - 1; y >= 0 ; y--) {
            for (int x = WINDOW_WIDTH - 1; x >= 0; x--) {
                if ((y / 32) % 2 == 0 && (x / 32) % 2 == 0) {
                    image_before.PutPixel(x, y, Pixel{.r = image_after.GetPixel(x, y).r, .g = image_after.GetPixel(x, y).g,
                            .b = image_after.GetPixel(x, y).b, .a = image_after.GetPixel(x, y).a});
                } else if ((y / 32) % 2 == 1 && (x / 32) % 2 == 1) {
                    image_before.PutPixel(x, y, Pixel{.r = image_after.GetPixel(x, y).r, .g = image_after.GetPixel(x, y).g,
                            .b = image_after.GetPixel(x, y).b, .a = image_after.GetPixel(x, y).a});
                }
            }

            if (y % tileSize == 0) {
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                GL_CHECK_ERRORS;
                glDrawPixels(WINDOW_WIDTH, WINDOW_HEIGHT, GL_RGBA, GL_UNSIGNED_BYTE, image_before.Data());
                GL_CHECK_ERRORS;
                glfwSwapBuffers(window);
            }
        }

        for (int x = WINDOW_WIDTH - 1; x >= 0; x--) {
            for (int y = WINDOW_HEIGHT - 1; y >= 0 ; y--) {
                if ((y / 32) % 2 == 0 && (x / 32) % 2 == 1) {
                    image_before.PutPixel(x, y, Pixel{.r = image_after.GetPixel(x, y).r, .g = image_after.GetPixel(x, y).g,
                            .b = image_after.GetPixel(x, y).b, .a = image_after.GetPixel(x, y).a});
                } else if ((y / 32) % 2 == 1 && (x / 32) % 2 == 0) {
                    image_before.PutPixel(x, y, Pixel{.r = image_after.GetPixel(x, y).r, .g = image_after.GetPixel(x, y).g,
                            .b = image_after.GetPixel(x, y).b, .a = image_after.GetPixel(x, y).a});
                }
            }
            if (x % tileSize == 0) {
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                GL_CHECK_ERRORS;
                glDrawPixels(WINDOW_WIDTH, WINDOW_HEIGHT, GL_RGBA, GL_UNSIGNED_BYTE, image_before.Data());
                GL_CHECK_ERRORS;
                glfwSwapBuffers(window);
            }
        }

    }
}// mozaik


///////////////Д Л Я  О Т Л А Д К И//////////////

void Game::printTXT_map() {
    std::cout << std::endl;
    for (std::string s : lvl){
        std::cout << s << std::endl;
    }
}

void Game::print_actionmap() {
    for (int y = 0; y < WINDOW_HEIGHT/tileSize; y++ ){
        for (int x = 0; x< WINDOW_WIDTH/tileSize; x++){
            std::cout<<actionmap[x][y]<<' ';
        }
        std::cout<<std::endl;
    }
}

/////////////////////////////////////////////////


