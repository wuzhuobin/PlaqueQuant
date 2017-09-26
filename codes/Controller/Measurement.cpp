#include "Measurement.h"

#include <vtkImageData.h>
#include <vtkCallbackCommand.h>

Measurement::Measurement(QObject * parent)
	: QObject(parent)
{
	m_callback = vtkCallbackCommand::New();
	m_callback->SetClientData(this);
	m_callback->SetCallback([](vtkObject *caller, unsigned long eid,
		void *clientdata, void *calldata) {
		Measurement* self = static_cast<Measurement*>(clientdata);
		self->update();
	});
}

Measurement::~Measurement()
{
	m_callback->Delete();
}

void Measurement::setOverlay(vtkImageData* overlay)
{
	if(m_overlay != overlay){
		if (m_overlay) {
			m_overlay->RemoveObserver(m_callback);
		}
		m_overlay = overlay;
		m_overlay->AddObserver(vtkCommand::UpdateDataEvent, m_callback);
		//disconnect(m_overlay, SIGNAL(signalUpdatedOverlay()),
		//	this, SLOT(update()));
		m_overlay = overlay;
		update();
	}
}

vtkImageData* Measurement::getOverlay()
{
	return m_overlay;
}

void Measurement::update()
{
	//emit updated();
}
