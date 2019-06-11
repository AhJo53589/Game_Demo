#include "Camera.h"
#include "Define2DRPG.h"

void Camera::addListener(CameraMoveEventListener *l)
{
	if (l)
	{
		this->mListenerList.push_back(l);
	}
}

void Camera::removeListener(CameraMoveEventListener *l)
{
	for(std::vector<CameraMoveEventListener *>::iterator it=this->mListenerList.begin(); it!=this->mListenerList.end(); it++) 
	{
		if(*it == l)
		{
			this->mListenerList.erase(it);
			return;
		}
	}
}

void Camera::notifyCameraMoveEvent()
{
	for(std::vector<CameraMoveEventListener *>::iterator it=this->mListenerList.begin(); it!=this->mListenerList.end(); it++) 
	{
		(*it)->CameraMoveEvent(this, this->mPos);
	}
}