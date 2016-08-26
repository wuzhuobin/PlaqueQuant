/*
Author:		Wong, Matthew Lun
Date:		26th, May 2016
Occupation:	Chinese University of Hong Kong,
Department of Imaging and Inteventional Radiology,
Junior Research Assistant


The abstract data class is designed to hold data structure typically used in a medical viewer.
This class is the ultimate parent of data structure.


Wong Matthew Lun
Copyright (C) 2016
*/

#ifndef ABSTRACT_DATA_H
#define ABSTRACT_DATA_H

#ifdef QT5_IS_LOADED
#include <QString>
#endif
#include <string>
using namespace std;

class AbstractData
{
public:
	AbstractData();
	~AbstractData();

	virtual void SetState(int);

#ifdef QT5_IS_LOADED
	void SetUniqueName(QString);
	QString	GetUniqueName();
#else
	void SetUniqueName(string);
	string	GetUniqueName();
#endif
	void SetVisibility(bool);


	bool	GetVisibility();
	bool	IsVisible();

	enum States
	{
		INTERMEDIATE = -1,
		IDLE = 0,
		HOVERED = 1,
		SELECTED = 2
	};

	// This should be moved to a new file
	enum DataType
	{
		MESH = 0,
		LANDMARK = 1,
		GUIDE_TUBE = 2, 
		IMAGE = 3,
	} m_dataType;

protected:
	int m_actorState;
#ifdef QT5_IS_LOADED
	QString m_uniqueName;
#else 
	string m_uniqueName;
#endif
	bool m_visibility;
};

#endif
