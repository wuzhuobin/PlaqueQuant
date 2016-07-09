#pragma once
#include <vtkCommand.h>

class MyVtkCommand :
	public vtkCommand
{
public:
	static enum EventIds {
		SliceChangeEvent = 1001

	};
	//MyVtkCommand();
	//~MyVtkCommand();
};

