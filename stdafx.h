// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"
#define WIN32_
// Windows Header Files:
#include "windows.h"
#include "window.h"

#include <GL/glew.h>
#include "GL/gl.h"
#include "GL/glu.h"

#include <iostream>

#include <fstream>
#include <stdexcept>
#include <string>
#include <vector>
#include <cstring>
#include <ctime>
#include <sstream>

#include "utilities.h"
#include "Bitmap.h"
#include "eventHandling.h"
#include "Tools.h"

#include "Model.h"
#include "GameObject.h"
#include "Terrain.h"
#include "LevelManager.h"
#include "Renderer.h"
#include "Player.h"
#include "ModelManager.h"
#include "Engine.h"
#include "UI.h"


// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
