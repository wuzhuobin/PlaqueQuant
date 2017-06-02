#ifndef __QINTERACTOR_STYLE_THRESHOLD_H__
#define __QINTERACTOR_STYLE_THRESHOLD_H__
/**
* @file	QInteractorStyleThreshold.h
* @brief	whatever
* @authour	wuzhuobin
* @date	20/12/2016
* @version	v1.0.0
*
* <pre>copyright:	authors have all right reserved. <pre/>
* <pre>email:		jiejin2022@163.com. <pre/>
* <pre>company:	CUHK. <pre/>
*/
#include "QAbstractNavigation.h"
#include "InteractorStyleThreshold.h"

namespace Ui {class QInteractorStyleThreshold;}
/**
* @class	QInteractorStyleThreshold
* @brief	window level.
* using cursor to change window level.
*/
class QSpinBox;
class QSlider;
class QPushButton;
class QLabel;
class QInteractorStyleThreshold : public QAbstractNavigation,
	public InteractorStyleThreshold
{
	Q_OBJECT;
	QSETUP_UI_HEAD(QInteractorStyleThreshold);

public:
	vtkTypeMacro(QInteractorStyleThreshold, InteractorStyleThreshold)
	static QInteractorStyleThreshold* New();
public slots:
	/**
	* @brief	Enabled/Disable this InteractorStyle.
	* @param	flag	true, enable. false, disable.
	*/
	virtual void SetCustomEnabled(bool flag);
	/**
	* @override
	* @brief	function to set the focal point.
	* @param	ijk	int[3] array to set the focal point
	* @param	i, j, k	to set the focal point
	*/
	virtual void SetCurrentFocalPointWithImageCoordinate(int i, int j, int k);

	/**
	 * @override
	 * @brief	function to set the window width spinbox
	 */
	virtual void SetLowerThreshold(int window);

	/**
	* @override
	* @brief	function to set the window level spinbox
	*/
	virtual void SetUpperThreshold(int level);

	virtual void ThresholdTargetViewerToOverlay();
	virtual void SetOutputLabel(int label) override;

	virtual void SetThresholdByViewer(double lower, double upper);
	virtual void ResetWindowLevel() override;
	virtual void SetPreview(bool flag) override;


protected:

	/**
	* @brief	Constructor.
	* @param	uiType	NO_UI
	* @param	parent	QWidget's parent
	*/
	QInteractorStyleThreshold(int uiType = UNIQUE_UI, QWidget * parent = Q_NULLPTR);
	/**
	 * @brief	Destructor. 
	 */
	virtual ~QInteractorStyleThreshold();

	virtual void uniqueEnable() override;

	void UpdateTargetViewer();





private:
	void initialization();
	void uniqueInitialization();
	///< the pointer of the navigation ui.
	Ui::QInteractorStyleThreshold *ui = nullptr;
	QLabel* m_label = nullptr;
	QSpinBox* m_spinBoxUpperThreshold = nullptr;
	QSpinBox* m_spinBoxLowerThreshold = nullptr;
	QSlider* m_sliderUpperThreshold = nullptr;
	QSlider* m_sliderLowerThreshold = nullptr;
	QPushButton* m_pushButtonReset = nullptr;

};


#endif // !__QINTERACTOR_STYLE_THRESHOLD_H__
