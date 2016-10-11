#ifndef DCENGINEACTIVEOBJECT_H_
#define DCENGINEACTIVEOBJECT_H_

#include <e32base.h>
#include <e32cons.h>
#include <e32std.h>
#include <EIKENV.H>
#include <W32STD.H>
#include <COEMAIN.H>

#include "MDCEngineWaitObserver.h"
#include "DCConstants.h"

class CDCEngineActiveObject;
class MDCEngineWaitObserver;

class CDCEngineActiveObject : public CActive
	{
	public:
		CDCEngineActiveObject( MDCEngineWaitObserver& aObserver );
		static CDCEngineActiveObject* NewL( MDCEngineWaitObserver& aObserver );
		~CDCEngineActiveObject();

	public:
		void IssueRequest( TInt aProCount );
		void DoCancel();
		void RunL();
		void ConstructL();
		void ThreadSuspend( TInt aReason );
		void SetLoopMaxVal( TInt aVal );

	public:
		TRequestStatus 					iNewStatus;
		MDCEngineWaitObserver& 			iObserver;
		RThread 						iActThread;
		RThread 						iThisThread;
		TInt 							iCount;
		CActiveSchedulerWait* 			iWait;
		TBool 							iFirstCall;
		TInt	 						iLoopMaxVal;
		TInt							iMMCRemove;
		TInt 							iMMCRemoveStatus;

	static TInt NewThread( TAny* aPkg );
	};

#endif /* DCENGINEACTIVEOBJECT_H_ */
