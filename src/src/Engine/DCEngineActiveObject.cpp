#include <avkon.hrh>
#include <aknnotedialog.h>
#include <aknenv.h>

#include "DCEngineActiveObject.h"

CDCEngineActiveObject* CDCEngineActiveObject::NewL( MDCEngineWaitObserver&
															aObserver )
	{
	CDCEngineActiveObject* self = new (ELeave) CDCEngineActiveObject(
																aObserver );
	self->ConstructL();
	return self;
	}

void CDCEngineActiveObject::ConstructL()
	{
	iFirstCall = ETrue;
	iLoopMaxVal = 0;
	}

CDCEngineActiveObject::CDCEngineActiveObject( MDCEngineWaitObserver&
														aObserver )
	: CActive( CActive::EPriorityIdle ),
	iObserver( aObserver )

	{
		CActiveScheduler::Add( this );
		iThisThread.Open( RThread().Id() );
		iActThread.Create( _L("ProgressThread"), CDCEngineActiveObject::
									NewThread, KDefaultStackSize, NULL, this );
	}

CDCEngineActiveObject::~CDCEngineActiveObject()
	{
    iActThread.Terminate( KErrNone );
	Cancel();
	}

void CDCEngineActiveObject::ThreadSuspend( TInt aReason )
	{
	iMMCRemoveStatus = aReason;
 	TRequestStatus* requestStatus = &( iStatus );
	iThisThread.RequestComplete( requestStatus, aReason );
 	iActThread.Suspend();
	}

void CDCEngineActiveObject::DoCancel()
	{
	}

void CDCEngineActiveObject::IssueRequest(TInt aProCount)
	{
	iCount = aProCount;
	iStatus = KRequestPending;
	SetActive();
	iActThread.Resume();
	}

void CDCEngineActiveObject::RunL()
	{
	if( iStatus == KErrNone && iCount <= iLoopMaxVal )
		{
		iObserver.MakeProgress();
		}
	else if( iStatus == EAknSoftkeyCancel )
		{

		iObserver.ContinueProgress();
		}
	else if( iMMCRemoveStatus == KPhoneCallEventIndicator )
		{
		//User::WaitForRequest(iStatus);
		iObserver.FinishProgress();
		iMMCRemoveStatus = 0;
		//iCount = iLoopMaxVal;
		//iActThread.Suspend();
		}

	else
		{
		iObserver.ThreadNotify();
		}
	}

TInt CDCEngineActiveObject::NewThread( TAny* aPkg )
	{
	RThread thisThread;
	while( ETrue )
		{
		CDCEngineActiveObject* active = static_cast
									<CDCEngineActiveObject*>( aPkg );
		TRequestStatus* requestStatus = &( active->iStatus );
		( active->iThisThread).RequestComplete(requestStatus, KErrNone );
		thisThread.Suspend();
		}
	}

void CDCEngineActiveObject::SetLoopMaxVal( TInt aVal )
	{
	iLoopMaxVal = aVal;
	}
