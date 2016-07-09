#pragma once
#include <vtkCommand.h>

class MyVtkCommand :
	public vtkCommand
{
public:
	static enum EventIds {
		SliceChangeEvent = 1001,
		WindowLevelEvent2 = 1002

	};
	//MyVtkCommand();
	//~MyVtkCommand();
};

