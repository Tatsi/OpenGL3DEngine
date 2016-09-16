#ifndef EVENT_HANDLING_H
#define EVENT_HANDLING_H

LRESULT CALLBACK MsgHandle(   HWND hwnd,      // "handle" to the window that this message is for
                            UINT message,   // TYPE of message (e.g. WM_PAINT is a message asking to paint the window)
                            WPARAM wparam,  // information about the actual message
                            LPARAM lparam ); // MORE info about the message

void keyPressed(char wParam);
void keyReleased(char wParam);
void mouseMoved(char wparam, char lparam);

#endif