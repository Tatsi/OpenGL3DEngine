#include "stdafx.h"
#include "UI.h"

UI UI::instance = UI();

void UI::init(HDC deviceCxtHandle)
{
	deviceContextHandle = deviceCxtHandle;

	for (int i = 0; i < GAME_LOG_SIZE; i++) //Init log with empty strings
	{
		gameLog.push_back("");
	}

	HFONT gameLogFont = CreateFont(30, 0, 0, 0,FW_NORMAL, FALSE, FALSE, FALSE,ANSI_CHARSET, 0,0,0,0,"Times New Roman");

	SelectObject(deviceContextHandle, gameLogFont);
	
	wglUseFontBitmaps(deviceContextHandle,0,127,1000);
}

void UI::draw()
{
	drawGameLog();
}

void UI::writeToGameLog(std::string text)
{
	gameLog.push_back(text);
	gameLog.erase(gameLog.begin());
}

void UI::drawGameLog()
{
	glColor3f(0.0, 0.5, 0.0);

	float x = Player::get().getX()-0.7*Player::get().getVX() + 0.12 *Player::get().getVZ();
	float z = Player::get().getZ()-0.7*Player::get().getVZ() - 0.12 *Player::get().getVX();

	for (int i = 0; i < GAME_LOG_SIZE; i++)
	{
		glRasterPos3f(x, Player::get().getY()+0.2 - i * 0.03, z);
		glListBase(1000);
		glCallLists(gameLog[i].length(), GL_UNSIGNED_BYTE, gameLog[i].c_str()); 
	}
}


