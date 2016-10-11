#include <eikmenub.h>
#include <akncontext.h>
#include <akntitle.h>
#include <stringloader.h>
#include <barsread.h>
#include <eikbtgpc.h>
#include <aknnotewrappers.h>
#include <aknglobalnote.h>
#include <S60DriveCleanser_0x20027916.rsg>

#include "DCEngine.h"
#include "DCEngineCore.h"
#include "DCEngineUtil.h"
#include "DCConstants.h"
#include "DCEngineProgress.h"

CDCEngine::CDCEngine()
	{
	}

CDCEngine::~CDCEngine()
	{
	delete iEngineCore;
	delete iEngineUtil;
	delete iActiveObject;
	delete iProgressDlg;

	iFullClean = EFalse;
	iIsInProgress = EFalse;
	}

CDCEngine* CDCEngine::NewLC()
	{
	CDCEngine* self = new ( ELeave )CDCEngine;
	CleanupStack::PushL( self );
	self->ConstructL();
	return self;
	}

CDCEngine* CDCEngine::NewL()
	{
	CDCEngine* self=CDCEngine::NewLC();
	CleanupStack::Pop();
	return self;
	}

void CDCEngine::ConstructL()
	{
	iEngineCore = CDCEngineCore::NewL();
	iEngineUtil = CDCEngineUtil::NewL();
	iActiveObject = CDCEngineActiveObject::NewL( *this );
	iProgressDlg = CDCEngineProgress::NewL( *iActiveObject );

	iCounter = 0;
	iTotalThread = 0;
	iIsInProgress = EFalse;
	iFullClean = EFalse;
	iDriveId = 0;

	iFolderToWrite.SetLength( 0 );
	iFolderToDelete.SetLength( 0 );
	}

void CDCEngine::SetDriveId( TInt aDriveId )
	{
	iDriveId = aDriveId;
	}

void CDCEngine::InitFolderToWrite()
	{
	TFileName folderName = iEngineUtil->DirectoryPath( iDriveId );
	iEngineUtil->MakeDirectory( folderName );
	iFolderToWrite.Copy( folderName );
	iEngineCore->MaximumFileSize( iDriveId );
	}

void CDCEngine::WriteFile( TInt aFile )
	{
	iFolderToWrite = CreateFile( aFile );
	iEngineCore->WriteFile( iFolderToWrite );
	}

void CDCEngine::DeleteFolder()
	{
	TFileName folderName = iEngineUtil->DirectoryPath( iDriveId );
	iEngineCore->DeleteFolder( folderName );
	}

void CDCEngine::DeleteExistingFolder()
	{
	for ( TInt driveNumber = EDriveC; driveNumber <= EDriveE; driveNumber++ )
		{
		TFileName folderName = iEngineUtil->DirectoryPath( driveNumber );
		iEngineCore->DeleteFolder( folderName );
		}
	}

void CDCEngine::ThreadCall()
	{
	iTotalThread = TotalThread();
	iActiveObject->SetLoopMaxVal( iTotalThread );
	InitFolderToWrite();
	iIsInProgress = ETrue;

	if( iFullClean == EFalse )
		{
		ShowProcessDialogForClean( iTotalThread );
		}
	else
		{
		ShowProcessDialogForFormat( iTotalThread );
		}
	}

void CDCEngine::PartialClean()
	{
	//iEngineCore->CheckCharger();
	if( CheckBatteryCharge( KRequiredChargeForPartialClean ) || (iEngineCore->GetChragerStatus()) )
		{
		TBool answer = ShowQueryDialogL( KMsgForPartClean );
		if( answer )
			{
			ShowConfirmationMsgL( KInfoMsg );
			iFullClean = EFalse;
			ThreadCall();
			}
		}
	else
		{
		ShowConfirmationMsgL( KMsgForMinBatteryLevel );
		}
	}

void CDCEngine::FullClean( )
	{
	//iEngineCore->CheckCharger();
	if( CheckBatteryCharge( KRequiredChargeForFullClean ) || (iEngineCore->GetChragerStatus()) )
		{
		RFs fs;
		User::LeaveIfError( fs.Connect() );
		iFullClean = ETrue;

		if( iDriveId == EDriveC )
			{
			TBool answer = ShowQueryDialogL( KMsgForPhoneMemory );
			if( answer )
				{
				iEngineCore->DeleteAllFilesAndFolder( fs, _L("C:\\") );
				ThreadCall();
				}
			}
		else if( iDriveId == EDriveE )
			{
			TBool answer = ShowQueryDialogL( KMsgForExtMemory );
			if( answer )
				{
				iEngineCore->DeleteAllFilesAndFolder( fs,  _L("E:\\") );
				ThreadCall();
				}
			}

		fs.Close();
		}
	else
		{
		ShowConfirmationMsgL( KMsgForMinBatteryLevel );
		}
	}

TInt CDCEngine::TotalThread()
	{
	TInt totalThread = KMaximumFile + KDeleteOperation;
	return totalThread;
	}

TBuf<KBufSize> CDCEngine::CreateFile( TInt aFile )
	{
	iFolderToWrite.Copy( _L("") );
	TFileName folderName = iEngineUtil->DirectoryPath( iDriveId );
	iFolderToWrite.Append( folderName );
	iFolderToWrite.AppendNum( aFile );
	return iFolderToWrite;
	}

TBuf<KMaxDrives> CDCEngine::GetAvailableDriveList()
	{
	return iEngineUtil->RetriveDriveList();
	}

TInt64 CDCEngine::GetDriveFreeSpace( TInt aDriveId )
	{
	return iEngineCore->DriveFreeSpace( aDriveId );
	}

TInt64 CDCEngine::GetDriveTotalSpace( TInt aDriveId )
	{
	return iEngineCore->DriveTotalSpace( aDriveId );
	}

void CDCEngine::ShowProcessDialogForClean( TInt aTotalThread )
	{
	iProgressDlg->StartProgressNoteL( aTotalThread, R_PROGRESSNOTE_CLEAN );
	DisplayText();
	iProgressDlg->UpdateProcessL( iCounter, iText );
	iCounter++;
	iActiveObject->IssueRequest( iCounter );
	}

void CDCEngine::ShowProcessDialogForFormat( TInt aTotalThread )
	{
	iProgressDlg->StartProgressNoteL( aTotalThread, R_PROGRESSNOTE_FORMAT );
	DisplayText();
	iProgressDlg->UpdateProcessL( iCounter, iText );
	iCounter++;
	iActiveObject->IssueRequest( iCounter );
	}

void CDCEngine::ContinueProgress()
	{
	if( ShowQueryDialogL( KMsgForAbortOperation ) )
		{
		DestroyProgressDialog();
		/*iCounter = 0;
		DeleteFolder();
		iTotalThread = 0;
		iIsInProgress = EFalse;*/
		}
	else
		{
		iIsInProgress = ETrue;
		//PartialClean();
		iFullClean = EFalse;
		ThreadCall();
		}
	}

void CDCEngine::DestroyProgressDialog()
	{
	iCounter = 0;
	DeleteFolder();
	iTotalThread = 0;
	iIsInProgress = EFalse;
	}

void CDCEngine::FinishProgress()
	{
	iProgressDlg->FinishProcessL();
	//iCounter = 0;
	//iTotalThread = 0;
	}

void CDCEngine::ThreadNotify()
	{
	iProgressDlg->FinishProcessL();
	iCounter = 0;
	iIsInProgress = EFalse;
	if( iFullClean )
		{
		ShowConfirmationMsgL( KMsgFullCleanComplete );
		//iEngineUtil->FactoryReset();
		}
	else
		{
		ShowConfirmationMsgL( KMsgPartCleanComplete );
		}
	}

void CDCEngine::MakeProgress()
	{
	//iEngineCore->CheckBatteryCharge();
	TUint currentCharge = iEngineCore->GetChargeLevel();

	if( currentCharge == KChargeForWarning && !( iEngineCore->GetChragerStatus() ) )
		{
		if( ShowQueryDialogL( KMSgForBatteryWarnMsg ) )
			{
			iIsInProgress = ETrue;
			PartialClean();
			}
		else
			{
			StopProgress();
			}
		}
	else if( currentCharge == KChargeForAbort && !( iEngineCore->GetChragerStatus() ) )
		{
		StopProgress();
		ShowConfirmationMsgL( KMsgAbortForLowBattery );
		}
	else
		{
		if( iCounter <= KMaximumFile )
			{
			TRAPD( err, WriteFile( iCounter ) );
			}

		if( iCounter > KMaximumFile )
			{
			DeleteFolder();
			}

		if( iFullClean )
			{
			DisplayText();
			iProgressDlg->UpdateProcessL( iCounter, iText );
			}
		else
			{
			DisplayText();
			iProgressDlg->UpdateProcessL( iCounter, iText );
			}

		iActiveObject->IssueRequest( iCounter );
		iCounter++;
		}
	}

void CDCEngine::ShowNoteL( const TDesC& aMessage ) const
	{
	CAknInformationNote* note = new( ELeave )CAknInformationNote;
	note->ExecuteLD( aMessage );
	}

TBool CDCEngine::ShowQueryDialogL( const TDesC& aMessage )
	{
	CAknQueryDialog* dlg = CAknQueryDialog::NewL();
	TBool answer( dlg->ExecuteLD( R_CONFIRMATION_QUERY, aMessage ) );
	return answer;
	}

void CDCEngine::ShowConfirmationMsgL( const TDesC& aMessage )
	{
	//CAknQueryDialog* dlg = CAknQueryDialog::NewL();
	//TBool answer( dlg->ExecuteLD( R_CONFIRMATION_ABORT, aMessage ) );
	//return answer;
	//CAknInformationNote* note = new( ELeave )CAknInformationNote;
	//note->ExecuteDlgLD( R_INFORMATION_NOTE, aMsgId );

	CAknGlobalNote* globalNote = CAknGlobalNote::NewLC();
	globalNote->SetSoftkeys( R_AVKON_SOFTKEYS_OK_EMPTY );
	globalNote->ShowNoteL( EAknGlobalInformationNote, aMessage );
	CleanupStack::PopAndDestroy();
	}

TUint CDCEngine::GetCurrentChargeLevel()
	 {
	 return iEngineCore->GetChargeLevel();
	 }

void CDCEngine::DisplayText()
	{
	TBuf<KBufSize>Percentage;
	TInt per = ( iCounter * KPercentage ) / iTotalThread;
	Percentage.Num( per );
	iText.SetLength( 0 );
	if( iFullClean )
		{
		iText.Copy( KFullCleanText );
		}
	else
		{
		iText.Copy( KPartialCleanText );
		}
	iText.Append( KNewLine );
	iText.Append( Percentage );
	iText.Append( KPercentageText );
	}

void CDCEngine::StopProgress()
	{
//	CEikonEnv::Static()->SetSystem( ETrue );
//
//	if(iIsInProgress)
//		{
//		iProgressDlg->FinishProcessL();
//
//
//		if( iActiveObject )
//			{
//			iActiveObject->ThreadSuspend( KMMCRemoveIndicator );
//			}
//		iIsInProgress = EFalse;
//		iCounter = 0;
//		iTotalThread = 0;
//		}
//	/*iCounter = 0;
//	iTotalThread = 0;*/
//	DeleteExistingFolder();
	}

TBool CDCEngine::CheckBatteryCharge( TInt aRequiredCharge )
	{
	//iEngineCore->CheckBatteryCharge();
	TUint charge = iEngineCore->GetChargeLevel();
	if ( charge >= aRequiredCharge )
		{
		return ETrue;
		}
	else
		{
		return EFalse;
		}
	}

void CDCEngine::PauseDialog()
	{
	if(iIsInProgress)
		{
		iActiveObject->ThreadSuspend( KPhoneCallEventIndicator );
		iIsInProgress = EFalse;
		}
	}

