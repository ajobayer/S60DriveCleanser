#include "DCEngineCheckBattery.h"

CDCEngineCheckBattery::CDCEngineCheckBattery( MBatteryObserver& aObserver )
: CActive( EPriorityStandard ),
  iObserver( aObserver ),
  iBatteryInfoV1Pckg( iBatteryInfoV1 )
	{
	 CActiveScheduler::Add( this );
	 
	}

CDCEngineCheckBattery::~CDCEngineCheckBattery()
	{
	Cancel();
	delete iTelephony;
	}

CDCEngineCheckBattery* CDCEngineCheckBattery::NewLC( MBatteryObserver&
																aObserver )
	{
	CDCEngineCheckBattery* self = new ( ELeave ) CDCEngineCheckBattery(
																aObserver );
	CleanupStack::PushL( self );
	self->ConstructL();
	return self;
	}

CDCEngineCheckBattery* CDCEngineCheckBattery::NewL(MBatteryObserver& aObserver)
	{
	CDCEngineCheckBattery* self = CDCEngineCheckBattery::NewLC( aObserver);
	CleanupStack::Pop();
	return self;
	}

void CDCEngineCheckBattery::ConstructL()
	{
	iTelephony = CTelephony::NewL();
	GetBatteryInfo();
	}

void CDCEngineCheckBattery::GetBatteryInfo()
	{
    if( iTelephony && !IsActive() )
    	{
        iGettingBattery = ETrue;
        iTelephony->GetBatteryInfo( iStatus, iBatteryInfoV1Pckg );
        //iTelephony->GetIndicator(iStatus, iIndicatorV1Pckg);    ///
        SetActive();
    	}
	}

void CDCEngineCheckBattery::RunL()
	{
    iObserver.BatteryChargeLevelL
					( iBatteryInfoV1.iChargeLevel, iBatteryInfoV1.iStatus );

    if( iStatus != KErrCancel )
    	{
        iTelephony->NotifyChange( iStatus,CTelephony::EBatteryInfoChange,
							iBatteryInfoV1Pckg );
        SetActive();
    	}
   
    
    iGettingBattery = EFalse;
	}

void CDCEngineCheckBattery::DoCancel()
	{
    if(	iGettingBattery	)
    	{
    	iTelephony->CancelAsync( CTelephony::EGetBatteryInfoCancel );
    	}
    else
    	{
    	iTelephony->CancelAsync( CTelephony::EBatteryInfoChangeCancel );
    	}
    //iTelephony->CancelAsync(CTelephony::EGetIndicatorCancel); ////
	}


