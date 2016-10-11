#ifndef __S60DRIVECLEANSERAPPLICATION_H__
#define __S60DRIVECLEANSERAPPLICATION_H__


#include <aknapp.h>
#include "S60DriveCleanser.hrh"

const TUid KUidS60DriveCleanserApp =
	{
	_UID3
	};

class CS60DriveCleanserApplication : public CAknApplication
	{
	public:
		TUid AppDllUid() const;

	protected:
		CApaDocument* CreateDocumentL();
	};

#endif

