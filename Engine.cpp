#include "stdafx.h"
#include "Engine.h"

Engine Engine::instance = Engine();

void Engine::init(HWND* _hwnd, int screen_max_resolution_vertical, int screen_max_resolution_horizontal)
{
	running = true;
	//#############################
	//##Reshape rendering context##
	//#############################
	hwnd = _hwnd;
	full_screen = false;
	_screen_max_resolution_vertical = screen_max_resolution_vertical;
	_screen_max_resolution_horizontal = screen_max_resolution_horizontal;
	reshape(800,600);
	previous_mouse_x = -5000;
	previous_mouse_y = -5000;
	lastRendered = 0.0;
	lastUpdated = 0.0;
	nextGameUpdateTime = 0.0;
	updateCount = 0;
}

void Engine::start()
{
	//Game State machine goes here
	LevelManager::get().loadLevel();
	UI::get().writeToGameLog("Entering main loop.");
	mainLoop();
	LevelManager::get().unloadLevel();
}

void Engine::quit()
{
	running = false;
}

void Engine::mainLoop()
{
	 while(running)
	 {
		//#####################
		//##Update game state##
		//#####################
		int numberOfConsecutiveUpdates = 0;

		while ( nextGameUpdateTime < float(clock()) / CLOCKS_PER_SEC && numberOfConsecutiveUpdates < MAX_CONSECUTIVE_GAME_UPDATES + 1)
		{
			updateCount++;
			numberOfConsecutiveUpdates++;

			//Measure game speed
			double ups = updateCount / (float(clock()) / CLOCKS_PER_SEC);//Calculate time difference
			std::string title = "JONgIne @ "; std::stringstream ss; ss << ups; title.append(ss.str()); title.append(" UPS");
			SetWindowText(*hwnd, (LPCTSTR)title.c_str());//Update UPS to window title

			if (updateCount % 3 == 0) //Move player 20 times per second
			{
				Player::get().move();
				Player::get().updateFalling();
				Renderer::get().setCameraY(Player::get().getY()+0.5); //Update camera position. Notice that camera is 0.5 higher than player center
			}
			
			nextGameUpdateTime += 1.0 / GAME_UPDATES_PER_SECOND;
				
			//###############################################################
			//##Check for events until there are no more (Every nth update)##
			//###############################################################
			if (updateCount % 3 == 0) //Handle keyboard input 20 times per second
			{
				MSG msg;
				while( PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE) )
				{
					PeekMessage( &msg, NULL, 0, 0 , PM_REMOVE);
					TranslateMessage( &msg );
					DispatchMessage( &msg ); //This calls MsgHandle function
				}
			}
			if (numberOfConsecutiveUpdates == MAX_CONSECUTIVE_GAME_UPDATES)
			{
				std::cout << "asa";
			}

			lastUpdated = clock();
		}
		//#############################
		//##Draw game and measure FPS##
		//#############################
		int time = clock();

		double fps = 1.0 / (float(time - lastRendered) / CLOCKS_PER_SEC);//Calculate time difference
		
		std::string title = "JONgIne @ "; std::stringstream ss; ss << fps; title.append(ss.str()); title.append(" FPS");

		//In order to get fps to the window I needed to Change character set from UNICODE to Multi-Byte
		//SetWindowText(*hwnd, (LPCTSTR)title.c_str());//Update FPS to window title

		Renderer::get().drawScene();
		lastRendered = clock();
	 }
}

void Engine::toggleFullscreen()
{
	if (full_screen) // Deactivate fullscreen
	{
		reshape(800,600);
		SetWindowPos(
			*hwnd,
			HWND_TOP,
			(_screen_max_resolution_vertical - 800) / 2,
			(_screen_max_resolution_horizontal - 600) / 2,
			800,
			600,
			SWP_SHOWWINDOW);
		full_screen = false;
	}
	else {
		reshape(_screen_max_resolution_vertical, _screen_max_resolution_horizontal);
		SetWindowPos(
			*hwnd,
			HWND_TOP,
			0,
			0,
			_screen_max_resolution_vertical,
			_screen_max_resolution_horizontal,
			SWP_SHOWWINDOW);
		full_screen = true;
	}
}

int Engine::getPreviousMouseX()
{
	return previous_mouse_x;
}
int Engine::getPreviousMouseY()
{
	return previous_mouse_y;
}
void Engine::setPreviousMouseX(int value)
{
	previous_mouse_x = value;
}
void Engine::setPreviousMouseY(int value)
{
	previous_mouse_y = value;
}