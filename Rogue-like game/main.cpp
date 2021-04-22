#include "common.h"
#include "Image.h"
#include "Player.h"
#include "Game.h"
#include "unistd.h"

#define GLFW_DLL
#include <GLFW/glfw3.h>


struct InputState
{
  bool keys[1024]{}; //массив состояний кнопок - нажата/не нажата
  GLfloat lastX = 400, lastY = 300; //исходное положение мыши
  bool firstMouse = true;
  bool captureMouse         = true;  // Мышка захвачена нашим приложением или нет?
  bool capturedMouseJustNow = false;
} static Input;


GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;


void OnKeyboardPressed(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	switch (key)
	{
	case GLFW_KEY_ESCAPE:
		if (action == GLFW_PRESS)
			glfwSetWindowShouldClose(window, GL_TRUE);
		break;
  case GLFW_KEY_1:
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    break;
  case GLFW_KEY_2:
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    break;
	default:
		if (action == GLFW_PRESS)
      Input.keys[key] = true;
		else if (action == GLFW_RELEASE)
      Input.keys[key] = false;
	}
}

void processPlayerMovement(Player &player)
{
  if (Input.keys[GLFW_KEY_W])
    player.ProcessInput(MovementDir::DOWN);
  else if (Input.keys[GLFW_KEY_S])
    player.ProcessInput(MovementDir::UP);
  if (Input.keys[GLFW_KEY_A])
    player.ProcessInput(MovementDir::LEFT);
  else if (Input.keys[GLFW_KEY_D])
    player.ProcessInput(MovementDir::RIGHT);
}

void OnMouseButtonClicked(GLFWwindow* window, int button, int action, int mods)
{
  if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE)
    Input.captureMouse = !Input.captureMouse;

  if (Input.captureMouse)
  {
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    Input.capturedMouseJustNow = true;
  }
  else
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

}

void OnMouseMove(GLFWwindow* window, double xpos, double ypos)
{
  if (Input.firstMouse)
  {
    Input.lastX = float(xpos);
    Input.lastY = float(ypos);
    Input.firstMouse = false;
  }

  GLfloat xoffset = float(xpos) - Input.lastX;
  GLfloat yoffset = Input.lastY - float(ypos);

  Input.lastX = float(xpos);
  Input.lastY = float(ypos);
}


void OnMouseScroll(GLFWwindow* window, double xoffset, double yoffset)
{
  // ...
}


int initGL()
{
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize OpenGL context" << std::endl;
		return -1;
	}

	std::cout << "Vendor: " << glGetString(GL_VENDOR) << std::endl;
	std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
	std::cout << "Version: " << glGetString(GL_VERSION) << std::endl;
	std::cout << "GLSL: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

  std::cout << "Controls: "<< std::endl;
  std::cout << "press right mouse button to capture/release mouse cursor  "<< std::endl;
  std::cout << "W, A, S, D - movement  "<< std::endl;
  std::cout << "press ESC to exit" << std::endl;

	return 0;
}

int main(int argc, char** argv)
{
	if(!glfwInit())
    return -1;

//	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
//	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
//	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

  GLFWwindow*  window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Task 1 (A3)", nullptr, nullptr);
	if (window == nullptr)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	
	glfwMakeContextCurrent(window); 

	glfwSetKeyCallback        (window, OnKeyboardPressed);  
	glfwSetCursorPosCallback  (window, OnMouseMove); 
	glfwSetMouseButtonCallback(window, OnMouseButtonClicked);
	glfwSetScrollCallback     (window, OnMouseScroll);

	if(initGL() != 0) 
		return -1;
	
  //Reset any OpenGL errors which could be present for some reason
	GLenum gl_error = glGetError();
	while (gl_error != GL_NO_ERROR)
		gl_error = glGetError();

    Image gamestart(gamestartImg_path);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); GL_CHECK_ERRORS;
    glRasterPos2f(-1, 1);
    glPixelZoom(1, -1);
    glDrawPixels (WINDOW_WIDTH, WINDOW_HEIGHT, GL_RGBA, GL_UNSIGNED_BYTE, gamestart.Data()); GL_CHECK_ERRORS;
    glfwSwapBuffers(window);
    
	Player player;

	Image screenBuffer(WINDOW_WIDTH, WINDOW_HEIGHT, 4);


  glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);  GL_CHECK_ERRORS;
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f); GL_CHECK_ERRORS;

    ///////////////////////////////////////////////////////////////////////////////////////////////////

    Game game;
    int level = 1;
    bool killed = false;
    game.set_lvl("../Resources/Levels/lvl_1");
    game.draw_lvl(screenBuffer, player);
    screenBuffer.Save(mapImg_path);
    Image map(mapImg_path);

    for (Enemy enemy : game.enemies){
        enemy.Draw(screenBuffer, map);
    }
    player.Draw(screenBuffer, map);

    sleep(0.5);
    game.mozaik(Image(gamestartImg_path), Image(mapImg_path), window, 1);


  //game loop
	while (!glfwWindowShouldClose(window))
	{
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
        glfwPollEvents();

        processPlayerMovement(player);
        player.colissionX(game.lvl); //столкновение со стенами по X
        player.colissionY(game.lvl); //столкновение со стенами по Y
        game.update(map,screenBuffer, player); //обновление карты: разруш-ся пол, анимация выхода
        for (int i = 0; i < game.enemies.size(); i++){ //движение врагов
            game.enemies[i].Draw(screenBuffer, map);
            game.enemies[i].move(game.lvl);
        }

        player.Draw(screenBuffer, map); //прорисовка пресонажа


        if (player.escaped(game.escape)){   //проверка на выход из лабиринта

        if (level == 1) {

            game.mozaik(Image(mapImg_path),Image(lvl2Img_path), window,0);
            Image load_lvl2(lvl2Img_path);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); GL_CHECK_ERRORS;
            glDrawPixels (WINDOW_WIDTH, WINDOW_HEIGHT, GL_RGBA, GL_UNSIGNED_BYTE, load_lvl2.Data()); GL_CHECK_ERRORS;
            glfwSwapBuffers(window);
            game.enemies.clear();
            game.set_lvl("../Resources/Levels/lvl_2");
            game.draw_lvl(screenBuffer, player);
            screenBuffer.Save(mapImg_path);
            game.mozaik(Image(lvl2Img_path),Image(mapImg_path), window, 1);
            map.set_image(mapImg_path);

            level++;

        }else
        if (level == 2){

            game.mozaik(Image(mapImg_path),Image(gameoverImg_path), window,0);
            Image gameoverImg(gameoverImg_path);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); GL_CHECK_ERRORS;
            glDrawPixels (WINDOW_WIDTH, WINDOW_HEIGHT, GL_RGBA, GL_UNSIGNED_BYTE, gameoverImg.Data());
            GL_CHECK_ERRORS;
            glfwSwapBuffers(window);
            level++;
            sleep(3);
            std::cout << "\n=== G A M E   R E S U L T S ===\nGAME OVER: You won!\n";
            std::cout << "\nGame created by A. Tverskoy\n" <<  "===============================\n";
            break;
        }
    }

    if (player.died(game.actionmap)){
            killed = true;
    }

    for (int i = 0; i < game.enemies.size(); i++){
        if (game.enemies[i].kills(player)){
            killed = true;
            break;
        }
    }

    if (killed){
        game.mozaik(Image(mapImg_path), Image(diedImg_path), window, 0);
        Image diedImg(diedImg_path);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); GL_CHECK_ERRORS;
        glDrawPixels (WINDOW_WIDTH, WINDOW_HEIGHT, GL_RGBA, GL_UNSIGNED_BYTE, diedImg.Data());
        GL_CHECK_ERRORS;
        glfwSwapBuffers(window);
        sleep(3);
        std::cout << "\n=== G A M E   R E S U L T S ===\nGAME OVER: You died! (level: "<< level << ")\n";
        std::cout << "\nGame created by A. Tverskoy\n" << "===============================\n";
        break;
    }

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); GL_CHECK_ERRORS;
        glDrawPixels (WINDOW_WIDTH, WINDOW_HEIGHT, GL_RGBA, GL_UNSIGNED_BYTE, screenBuffer.Data()); GL_CHECK_ERRORS;
		glfwSwapBuffers(window);

	} //gameloop

	glfwTerminate();
	return 0;
}
