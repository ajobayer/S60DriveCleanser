#include "DCEngineProgress.h"
#include "DCEngineActiveObject.h"

CDCEngineProgress::CDCEngineProgress( CDCEngineActiveObject& aActiveObj )
: iActiveObj( aActiveObj )
	{
	}

CDCEngineProgress::~CDCEngineProgress()
	{
	   if(iProgressDialog)
	   {
		  iProgressDialog->ProcessFinishedL();
	   }
	}

CDCEngineProgress* CDCEngineProgress::NewL( CDCEngineActiveObject& aActiveObj )
	{
	CDCEngineProgress* self=CDCEngineProgress::NewLC( aActiveObj );
	CleanupStack::Pop();
	return self;
	}

CDCEngineProgress* CDCEngineProgress::NewLC( CDCEngineActiveObject& aActiveObj )
	{
	CDCEngineProgress* self = new ( ELeave )CDCEngineProgress( aActiveObj );
	CleanupStack::PushL( self );
	self->ConstructL();
	return self;
	}

void CDCEngineProgress::ConstructL()
	{
	}

void CDCEngineProgress::StartProgressNoteL( TInt aFinalValue, TInt aResId )
{
   iProgressDialog = new (ELeave) CAknProgressDialog(
				(REINTERPRET_CAST(CEikDialog**, &iProgressDialog)), ETrue );
   iProgressDialog->PrepareLC( aResId );
   iProgressInfo = iProgressDialog->GetProgressInfoL();
   iProgressDialog->SetCallback(this);
   iProgressDialog->RunLD();
   iProgressInfo->SetFinalValue( aFinalValue );
}

void CDCEngineProgress::UpdateProcessL(TInt aProgress, const TDesC& aProgressText)
	{
	if( iProgressDialog )
		{
		iProgressDialog->SetTextL( aProgressText );
		}

	if( iProgressInfo )
		{
		iProgressInfo->SetAndDraw( aProgress );
		}
	}

void CDCEngineProgress::DialogDismissedL( TInt aReason )
	{
	if( iProgressDialog )
		{
		iProgressDialog = NULL;
		}

	if( aReason == EAknSoftkeyCancel )
		{
		iActiveObj.ThreadSuspend( EAknSoftkeyCancel );
		}

	/*if( aReason == KMMCRemoveIndicator )
		{
		iActiveObj.ThreadSuspend( KMMCRemoveIndicator );
		}*/
	}

void CDCEngineProgress::FinishProcessL()
	{
	if( iProgressDialog )
		{
		iProgressDialog->SetCallback(0);
     	iProgressDialog->ProcessFinishedL();
		}
	iProgressDialog = NULL;
	//iProgressDialog->ProcessFinishedL();
	}


