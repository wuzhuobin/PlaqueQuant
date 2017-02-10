#ifndef PLANNER_MACRO_H
#define PLANNER_MACRO_H

using namespace std;

#define ListIOMacro(data, type) \
void Append##data(##type* dat) \
{ \
	this->m_list##data.push_back(dat); \
} \
void Remove##data(int i) \
{ \
list<##type*>::iterator it = this->m_list##data.begin(); \
advance(it, i); \
this->m_list##data.erase(it); \
} \
void Remove##data(##type* dat) \
{ \
this->m_list##data.remove(dat); \
} \
list<##type*> GetList##data () \
{ \
	return this->m_list##data ; \
} \
##type* GetByUniqueName##data (QString* name) \
{ \
	list<##type*>::iterator it = this->m_list##data.begin(); \
	for (int i = 0; i < this->m_list##data.size();i++) \
	{ \
		if (*it->GetByUniqueName() == name) \
		{ \
			return this->m_list##data.at(i); \
		} \
		it++; \
	} \
	return NULL; \
}
#endif