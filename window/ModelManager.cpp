#include "stdafx.h"
#include "ModelManager.h"

ModelManager ModelManager::instance = ModelManager();

std::vector<Model>& ModelManager::getModels()
{
	return models;
}

Model& ModelManager::getModel(std::string name)
{
	for (std::vector<Model>::iterator it = models.begin(); it < models.end(); it++)
	{
		if (it->getName() == name)
		{
			return *it;
		}
	}
	std::string msg = "Model was not found by ModelManager. Model name: '";
	msg.append(name);
	msg.append("'. Exiting with code -1...\n");
	writeToLog("Model& ModelManager::getModel(std::string name)", msg);
	exit(-1);
}

void ModelManager::addModel(Model model)
{
	models.push_back(model);
}

void ModelManager::loadModels()
{
	for (std::vector<Model>::iterator it = models.begin(); it < models.end(); it++)
	{
		it->load();
	}
}

void ModelManager::clearLoadingData()
{
	for (std::vector<Model>::iterator it = models.begin(); it < models.end(); it++)
	{
		it->clearModelData();
	}
}

void ModelManager::removeModels()
{
	for (std::vector<Model>::iterator it = models.begin(); it < models.end(); it++)
	{
		it->unload();
	}
	models.clear();
}