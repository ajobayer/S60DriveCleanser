#include "S60DriveCleanser.hrh"
#include "S60DriveCleanserDocument.h"
#include "S60DriveCleanserApplication.h"

CApaDocument* CS60DriveCleanserApplication::CreateDocumentL()
	{
	return CS60DriveCleanserDocument::NewL( *this );
	}

TUid CS60DriveCleanserApplication::AppDllUid() const
	{
	return KUidS60DriveCleanserApp;
	}

