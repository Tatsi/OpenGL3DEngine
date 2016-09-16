#ifndef LEVEL_MANAGER_H
#define LEVEL_MANAGER_H

class LevelManager
{
public:
	static LevelManager& get() { return instance; }

	void loadLevel();//This currently supports only one level
	void unloadLevel();

	Terrain& getTerrain();

private:
	LevelManager() {}
	LevelManager(const LevelManager&);
	LevelManager& operator=(const LevelManager&);

	Terrain terrain;

	static LevelManager instance;
};

#endif