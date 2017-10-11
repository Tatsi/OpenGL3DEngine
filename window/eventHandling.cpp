#include "stdafx.h"

//######################################################################################
//##This function is called automatically when message is recieved by WinMain function##
//######################################################################################
LRESULT CALLBACK MsgHandle(   HWND hwnd,      // "handle" to the window that this message is for
                            UINT message,   // TYPE of message (e.g. WM_PAINT is a message asking to paint the window)
                            WPARAM wparam,  // information about the actual message
                            LPARAM lparam ) // MORE info about the message
{
	switch( message )
    {
		case WM_KEYDOWN:
			if (!(lparam & 0x40000000))//Check that key wasnt already pressed
			{
				keyPressed(wparam);
			}
		break;
		case WM_KEYUP:
			if (lparam & 0x40000000)//Check that key wasnt already pressed
			{
				keyReleased(wparam);
			}
		break;
		case WM_MOUSEMOVE:
			mouseMoved(wparam, lparam);
		break;
		case WM_SIZE:
			reshape(LOWORD(lparam), HIWORD(lparam));
		break;
		case WM_DESTROY:
			PostQuitMessage( 0 ) ;
        break;   
    }
	return DefWindowProc(hwnd, message, wparam, lparam);
}

void keyPressed(char wParam)
{
	std::string x;
	std::stringstream ss;
	switch (wParam)
	{
		case 87: //W
			Player::get().setMovingUp(true);
		break;
		case 83: //S
			Player::get().setMovingDown(true);
		break;
		case 65:
			Player::get().setMovingLeft(true);			
		break;
		case 68:
			Player::get().setMovingRight(true);
		break;
		case 70: //F = Full screen
			Engine::get().toggleFullscreen();
		break;
		case 80: //P = Print position
			x = "Current position is:\n";
			UI::get().writeToGameLog(x);
			x = "X="; 
			ss << Player::get().getX();
			x.append(ss.str()); x.append("\n");
			UI::get().writeToGameLog(x);
			x = "Y="; ss.seekp(0);
			ss << Player::get().getY();
			x.append(ss.str()); x.append("\n");
			UI::get().writeToGameLog(x);
			x = "Z="; ss.seekp(0);
			ss << Player::get().getZ();
			x.append(ss.str()); x.append("\n");
			UI::get().writeToGameLog(x);
		break;
		case VK_SPACE:
			Player::get().jump();
		break;
		case VK_ESCAPE:
			Engine::get().quit();
		break;
	}
}

void keyReleased(char wParam)
{
	switch (wParam)
	{
		case 87:
			Player::get().setMovingUp(false);
		break;
		case 83:
			Player::get().setMovingDown(false);
		break;
		case 65:
			Player::get().setMovingLeft(false);
		break;
		case 68:
			Player::get().setMovingRight(false);
		break;
	}
}

void mouseMoved(char wparam, char lparam)
{
	int xPos = LOWORD(lparam); 
    int yPos = HIWORD(lparam);//This is used later

	//THIS IS UGLY -5000 :SS It is used to check whether previous mouse values have not been inited
	if (Engine::get().getPreviousMouseX() != -5000)
	{
		int vx = xPos - Engine::get().getPreviousMouseX();
		if(vx > 0)
		{
			Renderer::get().rotateCameraRight();
		}
		else if (vx < 0)
		{
			Renderer::get().rotateCameraLeft();
		}
	}
	Engine::get().setPreviousMouseX(xPos);
	Engine::get().setPreviousMouseY(yPos);
}