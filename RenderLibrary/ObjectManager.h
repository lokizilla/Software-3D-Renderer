#ifndef OBJECTMANAGER_H
#define OBJECTMANAGER_H

#include "Object.h"
#include <iostream>
#include <vector>
#include "MD2Loader.h"

namespace RenderLibrary
{
	class ObjectManager
	{
	public:
		ObjectManager();
		~ObjectManager();

		int Initialise(); // Set up the manager
		int AddObject(const char* modelFilename); // Add a new object with the specified filename (model)
		int RemoveObject(int objectNumber); // Remove the specified object
		
		unsigned int index;
		
		int _totalObjects; // Total objects
		int _totalVerts; // Total verts

		RenderLibrary::MD2Loader* _modelLoader; // MD2 model loader: loads an .md2 model from disk

		std::vector<RenderLibrary::Object*> _objects;
	};
}

#endif // OBJECTMANAGER_H
