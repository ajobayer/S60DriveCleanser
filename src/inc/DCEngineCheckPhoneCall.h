#ifndef DCENGINECHECKPHONECALL_H
#define DCENGINECHECKPHONECALL_H

#include <e32base.h>
#include <e32std.h>
#include <etel3rdParty.h>

#include "MDCEnginePhoneCallObserver.h"

class MPhoneReceiverObserver;
class CDCEngineCheckPhoneCall : public CActive
	{
	public:
		CDCEngineCheckPhoneCall( MPhoneReceiverObserver& aPhoneReceiverObserver );
		static CDCEngineCheckPhoneCall* NewL( MPhoneReceiverObserver& aPhoneReceiverObserver );
		static CDCEngineCheckPhoneCall* NewLC( MPhoneReceiverObserver& aPhoneReceiverObserver );
		~CDCEngineCheckPhoneCall();
		void StartListening();
		void StartL();

	private:
		void ConstructL();
		void RunL();
		TInt RunError( TInt anError );
		void DoCancel();

	private:
		CTelephony* iTelephony;
		/*CTelephony::TCallStatusV1 iCallStatus;
		CTelephony::TCallStatusV1Pckg iCallStatusPkg;*/
		CTelephony::TCallInfoV1 		iCurrentCallInfo;
		CTelephony::TCallInfoV1Pckg		iCurrentStatusPckg;
		MPhoneReceiverObserver&			iPhoneReceiverObserver;
	};

#endif // DCENGINECHECKPHONECALL_H
