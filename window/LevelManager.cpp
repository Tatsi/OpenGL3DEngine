#include "stdafx.h"
#include "LevelManager.h"

LevelManager LevelManager::instance = LevelManager();

void LevelManager::loadLevel() // This currently supports only one scene
{
	// Load Scene 1

	// Select shaders
	std::string vertex_shader_name("Shaders/phong.vert");
	std::string fragment_shader_name("Shaders/phong.frag");

	// Add models that are used in this level
	ModelManager::get().addModel(Model("1box", vertex_shader_name, fragment_shader_name));
	//ModelManager::get().addModel(Model("bunny", vertex_shader_name, fragment_shader_name));
	ModelManager::get().addModel(Model("terrain", vertex_shader_name, fragment_shader_name));
	ModelManager::get().addModel(Model("knot", vertex_shader_name, fragment_shader_name));
	ModelManager::get().addModel(Model("player", vertex_shader_name, fragment_shader_name));
	ModelManager::get().loadModels();
	
	// Add terrain objects that form the terrain. All of these objects will be visible and collisionable in this level.
	vec3 origo;
	origo.x = 0.0; origo.y = 0.0; origo.z = 0.0;
	vec3 box_pos;
	box_pos.x = -2.0; box_pos.y = 3.5; box_pos.z = 0.0;
	vec3 knot_pos;
	knot_pos.x = 5.0; knot_pos.y = 3.0; knot_pos.z = 0.0;
	vec3 bunny_pos;
	bunny_pos.x = 1.0; bunny_pos.y = 3.0; bunny_pos.z = 0.0;

	vec3 zero_rotation;
	zero_rotation.x = 0.0f;
	zero_rotation.y = 0.0f;
	zero_rotation.z = 0.0f;

	terrain.addTerrainObject(GameObject("1box_1", ModelManager::get().getModel("1box"), box_pos, 1.0, zero_rotation, true, 1.0));
	//terrain.addTerrainObject(GameObject("bunny_1", ModelManager::get().getModel("bunny"), bunny_pos, 4.0, false, 0.6));
	terrain.addTerrainObject(GameObject("knot_1", ModelManager::get().getModel("knot"), knot_pos, 0.1, zero_rotation, true, 1.0));
	terrain.addTerrainObject(GameObject("terrain_1", ModelManager::get().getModel("terrain"), origo, 1.0, zero_rotation, true, 0.0));

	terrain.load(); // Store terrain vertices for use in collision detection
	ModelManager::get().clearLoadingData(); // Clear model_data from models, its no longer used

	Player::get().init();
}

void LevelManager::unloadLevel()
{
	ModelManager::get().removeModels();
	terrain.unload();
	Player::get().unload();
}

Terrain& LevelManager::getTerrain()
{
	return terrain;
}