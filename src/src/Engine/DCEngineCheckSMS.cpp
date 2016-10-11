#include <msvids.h>

#include "DCEngineCheckSMS.h"

const TMsvId KDrafts = KMsvDraftEntryId;
const TMsvId KInbox = KMsvGlobalInBoxIndexEntryId;

// When using the emulator the observed folder is Drafts, otherwise Inbox.
#ifdef __WINS__
    const TMsvId KObservedFolderId = KDrafts;
#else
    const TMsvId KObservedFolderId = KInbox;
#endif

CDCEngineCheckSMS::CDCEngineCheckSMS( MSMSEngineObserver* aObserver )
: iObserver( aObserver )
	{
	}

CDCEngineCheckSMS::~ CDCEngineCheckSMS()
	{
	if ( iMsvSession )
		{
		iMsvSession->Cancel();
		}
	delete iMsvSession;
	iMsvSession = NULL;
	}

CDCEngineCheckSMS* CDCEngineCheckSMS::NewLC( MSMSEngineObserver* aObserver )
	{
	CDCEngineCheckSMS* self = new (ELeave) CDCEngineCheckSMS( aObserver );
	CleanupStack::PushL( self );
	self->ConstructL();
	return self;
	}

CDCEngineCheckSMS* CDCEngineCheckSMS::NewL(MSMSEngineObserver* aObserver)
	{
	CDCEngineCheckSMS* self = CDCEngineCheckSMS::NewLC(aObserver);
	CleanupStack::Pop();
	return self;
	}

void CDCEngineCheckSMS::ConstructL()
	{
	iMsvSession = CMsvSession::OpenAsyncL(*this);
	}

void CDCEngineCheckSMS::HandleSessionEventL(TMsvSessionEvent aEvent,
			TAny* /*aArg1*/, TAny* aArg2, TAny* /*aArg3*/)
    {
    switch (aEvent)
        {
        case EMsvEntriesChanged:
            {
            if ( aArg2 && *(static_cast<TMsvId*>(aArg2)) == KObservedFolderId )
                {
                iObserver->SMSReceived();
                }
            break;
            }
        default:
			{
			break;
			}
        }
   }


