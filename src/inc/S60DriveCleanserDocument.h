#ifndef __S60DRIVECLEANSERDOCUMENT_h__
#define __S60DRIVECLEANSERDOCUMENT_h__

#include <akndoc.h>

class CS60DriveCleanserAppUi;
class CEikApplication;

class CS60DriveCleanserDocument : public CAknDocument
	{
	public:
		static CS60DriveCleanserDocument* NewL( CEikApplication& aApp );
		static CS60DriveCleanserDocument* NewLC( CEikApplication& aApp );
		virtual ~CS60DriveCleanserDocument();

	public:
		CEikAppUi* CreateAppUiL();

	private:
		void ConstructL();
		CS60DriveCleanserDocument( CEikApplication& aApp );
	};

#endif
