#ifndef ENGINE_H
#define ENGINE_H

#define GAME_UPDATES_PER_SECOND 60.0 //How many times to update game in one second
#define MAX_CONSECUTIVE_GAME_UPDATES 5 //This is the max amount of game updates without rendering
#define MAX_FPS 100.0
#define DEBUG false 

class Engine
{
public:
	static Engine& get() { return instance; }
	
	void init(HWND* _hwnd, int screen_max_resolution_vertical, int screen_max_resolution_horizontal);
	void start();
	void quit();
	void mainLoop();
	void toggleFullscreen();

	int getPreviousMouseX();
	int getPreviousMouseY();
	void setPreviousMouseX(int value);
	void setPreviousMouseY(int value);
private:
	Engine() { }
	Engine(const Engine &);
	Engine& operator=(const Engine&);

	HWND* hwnd;
	int _screen_max_resolution_vertical;
	int _screen_max_resolution_horizontal;
	float nextGameUpdateTime;
	int lastRendered; //Last time (in clock ticks) the scene was drawn
	int lastUpdated;//Last time (in clock ticks) the game was updated
	int updateCount; //How many times game has been updated in a row without rendering
	static Engine instance;
	bool running;
	int previous_mouse_x;
	int previous_mouse_y;
	bool full_screen; //Whether running in full_screen mode

};
#endif