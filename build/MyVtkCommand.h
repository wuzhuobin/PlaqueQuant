#pragma once
#include <vtkCommand.h>

class MyVtkCommand :
	public vtkCommand
{
public:
	static enum EventIds {
		SliceChangeEvent = 1001,
		EndWindowLevelEvent2 = 1002,
		WindowLevelEvent2 = 1003,
		NavigationEvent = 1004

	};
	//MyVtkCommand();
	//~MyVtkCommand();
};

