#include <e32base.h>
#include <e32cmn.h>
#include <f32file.h>

#include "DCEngineMMCCheck.h"

CDCEngineMMCCheck::CDCEngineMMCCheck( MMemoryCardObserver& aObserver ) :
CActive( EPriorityStandard ) , iObserver( aObserver )
	{
	}

CDCEngineMMCCheck* CDCEngineMMCCheck::NewLC( MMemoryCardObserver& aObserver )
	{
	CDCEngineMMCCheck* self = new (ELeave) CDCEngineMMCCheck( aObserver );
	CleanupStack::PushL(self);
	self->ConstructL();
	return self;
	}

CDCEngineMMCCheck* CDCEngineMMCCheck::NewL( MMemoryCardObserver& aObserver )
	{
	CDCEngineMMCCheck* self = CDCEngineMMCCheck::NewLC( aObserver );
	CleanupStack::Pop();
	return self;
	}

void CDCEngineMMCCheck::ConstructL()
	{
	CEikonEnv::Static()->SetSystem( ETrue );
	CActiveScheduler::Add( this );
	TRequestStatus* status = &iStatus;
	User::RequestComplete( status, KErrNone );
	SetActive();
	}

CDCEngineMMCCheck::~CDCEngineMMCCheck()
	{
	Cancel();
	}

void CDCEngineMMCCheck::DoCancel()
	{
	//CEikonEnv::Static()->FsSession().NotifyDismountCancel();
	CEikonEnv::Static()->FsSession().NotifyChangeCancel();

	CEikonEnv::Static()->FsSession().NotifyDiskSpaceCancel();
	}

TBool CDCEngineMMCCheck::HasCard()
	{
	TBool iHasMMC;
	iHasMMC = EFalse;
	RFs fileSession;
	fileSession.Connect();

	TDriveInfo i;
	if ( fileSession.Drive( i, EDriveE ) == KErrNone )
		{
		if ( i.iType != EMediaNotPresent &&
			 i.iType != EMediaUnknown &&
			 i.iType != EMediaCdRom &&
			 i.iType != EMediaRom )
			{

			iHasMMC = ETrue;
			}
		}
	fileSession.Close();
	return iHasMMC;
	}

void CDCEngineMMCCheck::RunL()
	{
	 if ( HasCard() )
		{
		iObserver.UpdateList();
		StartListenerRemove();
		}
	else
		{
		iObserver.UpdateList();
		StartListenerInsert();
		}
	}

TInt CDCEngineMMCCheck::RunError( TInt aError )
	{
	return aError;
	}

void CDCEngineMMCCheck::StartListenerRemove()
    {
    //CEikonEnv::Static()->FsSession().NotifyDismount( EDriveE, iStatus, EFsDismountForceDismount );
    CEikonEnv::Static()->FsSession().NotifyDiskSpace( KThreshold, EDriveE, iStatus );
    SetActive();
    }

void CDCEngineMMCCheck::StartListenerInsert()
	{
	CEikonEnv::Static()->FsSession().NotifyChange( ENotifyAll, iStatus, KPath );
	//iFileSession.NotifyChange( ENotifyAll, iStatus );
	SetActive();
	}
