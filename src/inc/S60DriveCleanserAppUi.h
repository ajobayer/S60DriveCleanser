#ifndef __S60DRIVECLEANSERAPPUI_h__
#define __S60DRIVECLEANSERAPPUI_h__

#include <aknappui.h>

#include "DCConstants.h"

class CS60DriveCleanserAppView;
class CDCEngine;

class CS60DriveCleanserAppUi : public CAknAppUi
	{
	public:
		void ConstructL();
		CS60DriveCleanserAppUi();
		virtual ~CS60DriveCleanserAppUi();

	private:
		void HandleCommandL( TInt aCommand );
		void HandleStatusPaneSizeChange();
		void InitVolumeInfo();

	private:
		CS60DriveCleanserAppView* 	iAppView;
		CDCEngine* 					iEngine;
		CArrayFix<TCoeHelpContext>* HelpContextL() const;
	};

#endif

