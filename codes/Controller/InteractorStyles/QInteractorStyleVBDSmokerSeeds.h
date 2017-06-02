#ifndef __QINTERACTOR_STYLE_VBD_SMOKER_SEEDS_H__
#define __QINTERACTOR_STYLE_VBD_SMOKER_SEEDS_H__

#include "InteractorStyleSeedsPlacer.h"
#include "QAbstractNavigation.h"

#include <vtkSmartPointer.h>


namespace Ui { class QInteractorStyleVBDSmokerSeeds; }

class vtkHandleWidget;
class QPushButton;

class QInteractorStyleVBDSmokerSeeds : public QAbstractNavigation, 
	public InteractorStyleSeedsPlacer
{
	Q_OBJECT;
	QSETUP_UI_HEAD(QInteractorStyleVBDSmokerSeeds);

public:
	const static int NUM_OF_ELONGATION = 3;
	const static int NUM_OF_DETOUR = 7;

	vtkTypeMacro(QInteractorStyleVBDSmokerSeeds, InteractorStyleSeedsPlacer);
	static QInteractorStyleVBDSmokerSeeds* New();
public slots:
	virtual void GenerateWidgetFromSeeds() override;
	virtual void slotUpdateBasilarArteryBifurcationLocation();
	virtual void slotUpdatePonsCentralSectionLocation();

	void slotBasilarArteryBifurcationLocationCurrentSlice();
	void slotPonsCentralSectionLocationCurrentSlice();
	void SetCustomEnabled(bool flag);
	void SetCurrentFocalPointWithImageCoordinate(int i, int j, int k);

	/**
	 * save all widgets into the 2 vector
	 * since all seeds has already created beforehand, no need to check and push
	 * juse replacing
	 */
	virtual void SaveWidgetToSeeds() override;
	/**
	 * @deprecated
	 * @oveerride
	 * just directly invoke #SaveWidgetToSeeds() and #GenerateWidgetFromSeeds()
	 * do nothing
	 */
	virtual void UpdateWidgetToSeeds(
		int* newImagePos,
		int* oldImagePos = nullptr) override;

signals:

	void elongationChanged(int i);
	void detourChanged(int i);

protected:
	QInteractorStyleVBDSmokerSeeds(int uiType = UNIQUE_UI, QWidget* parent = Q_NULLPTR);
	~QInteractorStyleVBDSmokerSeeds();

	/**
	* @deprecated
	* @oveerride
	* do nothing
	*/
	void UpdateWidgetToSeeds(
		std::vector<int*>& seeds,
		int* newImagePos,
		int* oldImagePos = nullptr);
	void ClearAllSeeds() override;


	virtual void OnLeftButtonDown() override;
	virtual void OnKeyPress() override;

	virtual void uniqueEnable() override;
	virtual void uniqueDisable() override;

	static std::vector<int*> m_VBDSmokerSeeds;
	
	void uniqueInitialization();
	void initialization();

private:
	void print();

	static QPushButton* pushButtons[NUM_OF_ELONGATION + NUM_OF_DETOUR];
	
	Ui::QInteractorStyleVBDSmokerSeeds* ui = nullptr;

};

#endif // !__QINTERACTOR_STYLE_VBD_SMOKER_SEEDS_H__
