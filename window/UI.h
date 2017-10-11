#ifndef UI_H
#define UI_H

#define GAME_LOG_SIZE 5

class UI
{
public:
	static UI& get() { return instance; }

	void init(HDC deviceCxtHandle);
	void draw(); //Draw UI
	void writeToGameLog(std::string text);
	void drawGameLog();

private:
	UI() {}
	UI(const UI&);
	UI& operator=(const UI&);

	static UI instance;
	HDC deviceContextHandle;

	std::vector<std::string> gameLog;
	HFONT gameLogFont;
};

#endif