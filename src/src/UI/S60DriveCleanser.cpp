#include <eikstart.h>

#include "S60DriveCleanserApplication.h"

LOCAL_C CApaApplication* NewApplication()
	{
	return new CS60DriveCleanserApplication;
	}

GLDEF_C TInt E32Main()
	{
	return EikStart::RunApplication( NewApplication );
	}

