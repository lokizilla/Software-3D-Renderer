#include "StdAfx.h"
#include "ObjectManager.h"

namespace RenderLibrary
{
	ObjectManager::ObjectManager()
	{
		_modelLoader = new RenderLibrary::MD2Loader();
		_totalObjects = 0;
		_totalVerts = 0;
	}

	ObjectManager::~ObjectManager()
	{
		for (index = 0; index < _objects.size(); index++) 
		{
			delete _objects[index];
		}

		_objects.clear();
	}

	int ObjectManager::Initialise()
	{
		return 0; // debug
	}

	int ObjectManager::AddObject(const char* filename)
	{
		_objects.push_back(new RenderLibrary::Object());
		_objects.back()->Initialise();
		_modelLoader->LoadModel(filename, *_objects.back());

		_totalObjects++; // Increase counter
		_totalVerts += _objects.back()->GetNumberOfVerts(); // Increase counter

		return 0; // debug
	}

	int ObjectManager::RemoveObject(int number)
	{
		_totalVerts -= _objects[number]->GetNumberOfVerts(); // Decrease counter
		
		delete _objects[number]; // Delete object
		_objects.erase(_objects.begin() + number); // Remove object

		_totalObjects--; // Decrease counter

		return 0; // debug
	}
}
