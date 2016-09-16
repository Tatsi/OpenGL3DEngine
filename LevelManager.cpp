#include "stdafx.h"
#include "LevelManager.h"

LevelManager LevelManager::instance = LevelManager();

void LevelManager::loadLevel()//This currently supports only one level
{
	//###Load level 1###

	//Add models that are used in this level
	ModelManager::get().addModel(Model("1box", "Shaders/basic.vert", "Shaders/basic.frag"));
	ModelManager::get().addModel(Model("bunny", "Shaders/basic.vert", "Shaders/basic.frag"));
	ModelManager::get().addModel(Model("terrain", "Shaders/basic.vert", "Shaders/basic.frag"));
	ModelManager::get().loadModels();
	
	//Add terrain objects that form the terrain. All of these objects will be visible and collisionable in this level.
	vec3 origo;
	origo.x = 0.0;origo.y = 0.0;origo.z = 0.0;
	vec3 box_pos;
	box_pos.x = 0.0;box_pos.y = 3.5;box_pos.z = 2.0;
	vec3 bunny_pos;
	bunny_pos.x = 1.0;bunny_pos.y = 3.0;bunny_pos.z = 0.0;

	terrain.addTerrainObject(GameObject("1box_1", ModelManager::get().getModel("1box"), box_pos, 1.0, true, 1.0));
	terrain.addTerrainObject(GameObject("bunny_1", ModelManager::get().getModel("bunny"), bunny_pos, 4.0, false, 0.6));
	terrain.addTerrainObject(GameObject("terrain_1", ModelManager::get().getModel("terrain"), origo, 1.0, true, 0.0));

	terrain.load(); //Store terrain vertices for use in collision detection
	ModelManager::get().clearLoadingData(); //Clear model_data from models, its no longer used

	Player::get().init();
}
void LevelManager::unloadLevel()
{
	ModelManager::get().removeModels();
	terrain.unload();
}

Terrain& LevelManager::getTerrain()
{
	return terrain;
}