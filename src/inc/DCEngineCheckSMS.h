#ifndef DCENGINECHECKSMS_H
#define DCENGINECHECKSMS_H

#include <e32std.h>
#include <e32base.h>
#include <msvapi.h>

#include "MDCEngineSMSObserver.h"

class MSMSEngineObserver;

class CDCEngineCheckSMS : public CBase, public MMsvSessionObserver
	{
	public:
		~CDCEngineCheckSMS();
		static CDCEngineCheckSMS* NewL( MSMSEngineObserver* aObserver );
		static CDCEngineCheckSMS* NewLC( MSMSEngineObserver* aObserver );

	public:
	   void HandleSessionEventL( TMsvSessionEvent aEvent,
										   TAny* aArg1,
										   TAny* aArg2,
										   TAny* aArg3 );

	private:
		CDCEngineCheckSMS( MSMSEngineObserver* aObserver );
		void ConstructL();

	private:
			MSMSEngineObserver* iObserver;
			CMsvSession* iMsvSession;
	};

#endif // DCENGINECHECKSMS_H
