#include "DCEngineCheckPhoneCall.h"

CDCEngineCheckPhoneCall::CDCEngineCheckPhoneCall( MPhoneReceiverObserver& aPhoneReceiverObserver ) :
    CActive( EPriorityStandard ),
    iCurrentStatusPckg(iCurrentCallInfo),
    iPhoneReceiverObserver(aPhoneReceiverObserver)/*,
        iCallStatusPkg( iCallStatus )*/
    {
    }

CDCEngineCheckPhoneCall* CDCEngineCheckPhoneCall::NewL( MPhoneReceiverObserver& aPhoneReceiverObserver )
    {
    CDCEngineCheckPhoneCall* self = CDCEngineCheckPhoneCall::NewLC( aPhoneReceiverObserver );
    CleanupStack::Pop( self );
    return self;
    }

CDCEngineCheckPhoneCall* CDCEngineCheckPhoneCall::NewLC( MPhoneReceiverObserver& aPhoneReceiverObserver )
    {
    CDCEngineCheckPhoneCall* self = new (ELeave) CDCEngineCheckPhoneCall( aPhoneReceiverObserver );
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }

void CDCEngineCheckPhoneCall::ConstructL()
    {
    iTelephony = CTelephony::NewL();
    CActiveScheduler::Add( this );
    StartL();
    }

void CDCEngineCheckPhoneCall::StartL()
	{
	Cancel();
	iTelephony->NotifyChange( iStatus, CTelephony::EVoiceLineStatusChange,
		iCurrentStatusPckg );
	SetActive();
	}

CDCEngineCheckPhoneCall::~CDCEngineCheckPhoneCall()
    {
    Cancel();

    if ( iTelephony )
    	{
    	delete iTelephony;
    	iTelephony = NULL;
    	}
    }

void CDCEngineCheckPhoneCall::RunL()
    {
    if( iStatus.Int() == KErrNone )
		{
		CTelephony::TCallStatus callStatus = iCurrentStatusPckg().iStatus;
		//CTelephony::TCallStatus status = iCallStatus.iStatus;

		switch ( /*status*/callStatus )
			{
			case CTelephony::EStatusRinging:
				{
				iPhoneReceiverObserver.CallEventHandler();
				break;
				}
			case CTelephony::EStatusAnswering:
				{
				 //iPhoneReceiverObserver.CallEventHandler();
				//iPhoneReceiverObserver.CallAnswered();
				break;
				}
			case CTelephony::EStatusDialling:
				{
				 iPhoneReceiverObserver.CallEventHandler();
				//iPhoneReceiverObserver.DialingCall();
				break;
				}
			case CTelephony::EStatusConnecting:
				{
				//iPhoneReceiverObserver.CallConnecting();
				//iPhoneReceiverObserver.CallEventHandler();
				break;
				}

			case CTelephony::EStatusConnected:
				{
				 //iPhoneReceiverObserver.CallEventHandler();
				//iPhoneReceiverObserver.CallConnected();
				break;
				}
			case CTelephony::EStatusDisconnecting:
				{
				 //iPhoneReceiverObserver.CallEventHandler();
				iPhoneReceiverObserver.CallDisConnected();
				break;
				}

			default:
				{
				break;
				}
			}
		StartListening();
		}
    }

TInt CDCEngineCheckPhoneCall::RunError( TInt anError )
    {
    return anError;
    }

void CDCEngineCheckPhoneCall::DoCancel()
    {
    iTelephony->CancelAsync( CTelephony::EVoiceLineStatusChangeCancel );
    }

void CDCEngineCheckPhoneCall::StartListening()
    {
    iTelephony->NotifyChange( iStatus, CTelephony::EVoiceLineStatusChange,
                /*iCallStatusPkg*/iCurrentStatusPckg );

    /*iTelephony->NotifyChange( iStatus, CTelephony::EVoiceLineStatusChange,
                iCallStatusPkg );*/
    SetActive();
    }
