#ifndef MODEL_MANAGER_H
#define MODEL_MANAGER_H

class ModelManager
{
public:
	static ModelManager& get() { return instance; }
	
	std::vector<Model>& getModels();
	Model& getModel(std::string name);

	void addModel(Model model);
	void loadModels();
	void removeModels();
	void clearLoadingData(); //Clear the model_data that was used in intialization from the models. 
							 //This data is required when terrain game objects are added in Terrain class 
							 //in order to form a group of terrain triangles for collision detection. So add terrain objects first!

private:
	ModelManager() {}
	ModelManager(const ModelManager&);
	ModelManager& operator=(const ModelManager&);

	std::vector<Model> models;

	static ModelManager instance;
};

#endif
