#include "aknnotewrappers.h"
#include "DCEngineChargerStatusCheck.h"

CDCEngineChargerStatusCheck::CDCEngineChargerStatusCheck( MChargerObserver& aObserver ) :
	CActive( EPriorityStandard ),
	iObserver( aObserver ),
	iIndicatorV1Pckg( iIndicatorV1 )
	{
	CActiveScheduler::Add( this );
	}

CDCEngineChargerStatusCheck* CDCEngineChargerStatusCheck::NewLC( MChargerObserver& aObserver )
	{
	CDCEngineChargerStatusCheck* self =
			new (ELeave) CDCEngineChargerStatusCheck( aObserver );
	CleanupStack::PushL(self);
	self->ConstructL();
	return self;
	}

CDCEngineChargerStatusCheck* CDCEngineChargerStatusCheck::NewL( MChargerObserver& aObserver )
	{
	CDCEngineChargerStatusCheck* self = CDCEngineChargerStatusCheck::NewLC( aObserver );
	CleanupStack::Pop(); // self;
	return self;
	}

void CDCEngineChargerStatusCheck::ConstructL()
	{
	iTelephony = CTelephony::NewL();
	//CActiveScheduler::Add(this); // Add to scheduler
	DetectCharger();
	}

CDCEngineChargerStatusCheck::~CDCEngineChargerStatusCheck()
	{
	Cancel(); 
	delete iTelephony;
	}
void CDCEngineChargerStatusCheck::DetectCharger()
    {
    iTelephony->GetIndicator( iStatus, iIndicatorV1Pckg );
    SetActive();
    }

void CDCEngineChargerStatusCheck::DoCancel()
	{
	iTelephony->CancelAsync( CTelephony::EGetIndicatorCancel );
	iTelephony->CancelAsync( CTelephony::EIndicatorChangeCancel);
	}

void CDCEngineChargerStatusCheck::RunL()
	{

	  if( iIndicatorV1.iCapabilities & CTelephony::KIndChargerConnected )
		  {
		  //We can detect when a charger is connected
		  if( iIndicatorV1.iIndicator & CTelephony::KIndChargerConnected )
			 {
			 iObserver.ChargerConnected();
			 }
		  else
			 {
			 iObserver.ChargerIsNotConnected();
			 }
		  ChargerListener();
		  }
	  
	   else
		  {
			
		  }

	}

void CDCEngineChargerStatusCheck::ChargerListener()
	{
	iTelephony->NotifyChange( iStatus, CTelephony::EIndicatorChange,
						 iIndicatorV1Pckg );
	SetActive();
	}

