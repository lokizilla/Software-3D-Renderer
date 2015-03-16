#ifndef MD2LOADER_H
#define MD2LOADER_H

#include "Object.h"

namespace RenderLibrary
{
	class MD2Loader
	{
	public:
		MD2Loader();
	public:
		~MD2Loader();

	public:
		bool LoadModel(const char* pFilename, RenderLibrary::Object&);
	};
}

#endif // MD2LOADER_H
