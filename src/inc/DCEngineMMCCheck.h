#ifndef DCENGINEMMCCHECK_H
#define DCENGINEMMCCHECK_H

#include <e32base.h>
#include <e32std.h>
#include <avkon.hrh>
#include <aknmessagequerydialog.h>
#include <f32file.h>
#include <s32file.h>
#include <aknnotewrappers.h>
#include "DCConstants.h"
#include "MDCMMCObserver.h"

_LIT( KPath,"E:\\");

class MMemoryCardObserver;

class CDCEngineMMCCheck : public CActive
	{
public:

	~CDCEngineMMCCheck();
	static CDCEngineMMCCheck* NewL(MMemoryCardObserver& aObserver);
	static CDCEngineMMCCheck* NewLC(MMemoryCardObserver& aObserver);

public:
	void StartListenerRemove();
	TBool HasCard();
	void StartListenerInsert();

private:
	CDCEngineMMCCheck(MMemoryCardObserver& aObserver);
	void ConstructL();

private:
	void RunL();
	void DoCancel();
	TInt RunError(TInt aError);

private:
	MMemoryCardObserver&	iObserver;
	TInt iState; // State of the active object
	RFs fs;
	RFs iFileSession;

};

#endif // DCENGINEMMCCHECK_H
