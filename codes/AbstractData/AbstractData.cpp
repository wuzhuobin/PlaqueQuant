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

#include "AbstractData.h"

AbstractData::AbstractData()
{
	m_actorState = IDLE;
	m_visibility = true;
}

#ifdef QT5_IS_LOADED
void AbstractData::SetUniqueName(QString uniqueName)
{
	m_uniqueName = uniqueName;
}

QString AbstractData::GetUniqueName()
{
	return m_uniqueName;
}

#else
void AbstractData::SetUniqueName(string uniqueName)
{
	m_uniqueName = uniqueName;
}

string AbstractData::GetUniqueName()
{
	return m_uniqueName;
}
#endif

void AbstractData::SetVisibility(bool vis)
{
	m_visibility = vis;
}


bool AbstractData::GetVisibility()
{
	return m_visibility;
}

bool AbstractData::IsVisible()
{
	return this->GetVisibility();
}

AbstractData::~AbstractData()
{
}

void AbstractData::SetState(int state)
{
	m_actorState = state;
}
