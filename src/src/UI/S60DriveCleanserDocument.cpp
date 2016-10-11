#include "S60DriveCleanserAppUi.h"
#include "S60DriveCleanserDocument.h"

CS60DriveCleanserDocument* CS60DriveCleanserDocument::NewL( CEikApplication& aApp )
	{
	CS60DriveCleanserDocument* self = NewLC(aApp);
	CleanupStack::Pop(self);
	return self;
	}

CS60DriveCleanserDocument* CS60DriveCleanserDocument::NewLC( CEikApplication& aApp )
	{
	CS60DriveCleanserDocument* self = new ( ELeave ) CS60DriveCleanserDocument( aApp );

	CleanupStack::PushL(self);
	self->ConstructL();
	return self;
	}

void CS60DriveCleanserDocument::ConstructL()
	{
	}

CS60DriveCleanserDocument::CS60DriveCleanserDocument( CEikApplication& aApp )
: CAknDocument( aApp )
	{
	}

CS60DriveCleanserDocument::~CS60DriveCleanserDocument()
	{
	}

CEikAppUi* CS60DriveCleanserDocument::CreateAppUiL()
	{
	return new ( ELeave )CS60DriveCleanserAppUi;
	}


