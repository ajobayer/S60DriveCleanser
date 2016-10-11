#ifndef DCENGINE_H
#define DCENGINE_H

#include <f32file.h>
#include <e32std.h>
#include <e32base.h>

#include "DCEngineActiveObject.h"
#include "MDCEngineWaitObserver.h"
#include "DCConstants.h"

class CDCEngineCore;
class CDCEngineUtil;
class CDCEngineProgress;

class CDCEngine : public CBase, public MDCEngineWaitObserver
	{
	public:
		~CDCEngine();
		static CDCEngine* NewL();
		static CDCEngine* NewLC();

	public:
		TBuf<KMaxDrives> GetAvailableDriveList();
		TInt64 GetDriveFreeSpace( TInt aDriveId );
		TInt64 GetDriveTotalSpace( TInt aDriveId );
		TInt ShowQueryDialogL( const TDesC& aMessage );
		TUint GetCurrentChargeLevel();

	public:
		void ShowProcessDialogForClean(TInt aVal);
		void ShowProcessDialogForFormat(TInt aVal);
		void ThreadNotify();
		void MakeProgress();
		void ContinueProgress();
		void DestroyProgressDialog();
		void FinishProgress();
		void UpdateProcessL(TInt aProgress,const TDesC& aProgressText);

	   	void ShowNoteL(const TDesC& aMessage) const;
	   	void DisplayText();

	   	void SetDriveId( TInt aDriveId );
	   	void WriteFile( TInt aFile );
	   	void DeleteFolder();
	   	void DeleteExistingFolder();
	   	void InitFolderToWrite();
	   	void ThreadCall();
	   	void PartialClean();
	   	void FullClean();
	   	TInt TotalThread();
	   	TBuf<KBufSize> CreateFile( TInt aFile );
	   	void ShowConfirmationMsgL( const TDesC& aMessage );
	   	void StopProgress();
	   	TBool CheckBatteryCharge( TInt aRequiredCharge );
		void PauseDialog();

	private:
		CDCEngine();
		void ConstructL();

	private:
		CDCEngineCore* 				iEngineCore;
		CDCEngineUtil* 				iEngineUtil;
		TBool						iFullClean;
		TInt 						iCounter;
		TBool						iIsInProgress;
		CDCEngineActiveObject* 		iActiveObject;
		TBuf<KBufSize>				iText;
		TInt						iTotalThread;
		TInt						iDriveId;
		TBuf<KBufSize>				iFolderToWrite;
		TBuf<KBufSize>				iFolderToDelete;
		CDCEngineProgress*			iProgressDlg;
		TBool						iPhonememoryFormat;
	};

#endif
