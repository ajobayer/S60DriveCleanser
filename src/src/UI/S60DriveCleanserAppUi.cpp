#include <avkon.hrh>
#include <aknmessagequerydialog.h>
#include <aknnotewrappers.h>
#include <stringloader.h>
#include <f32file.h>
#include <s32file.h>
#include <hlplch.h>
#include <BAUTILS.H>

#include <S60DriveCleanser_0x20027916.rsg>

#include "S60DriveCleanser_0x20027916.hlp.hrh"
#include "S60DriveCleanser.hrh"
#include "S60DriveCleanser.pan"
#include "S60DriveCleanserApplication.h"
#include "S60DriveCleanserAppUi.h"
#include "S60DriveCleanserAppView.h"
#include "DCEngine.h"

void CS60DriveCleanserAppUi::ConstructL()
	{
	BaseConstructL( CAknAppUi::EAknEnableSkin );

	iEngine = CDCEngine::NewL();
	iAppView = CS60DriveCleanserAppView::NewL( ClientRect(), iEngine );
	iAppView->SetMopParent( this );
	AddToStackL( iAppView );

	//CheckBatteryCharge();
	}

CS60DriveCleanserAppUi::CS60DriveCleanserAppUi()
	{
	}

CS60DriveCleanserAppUi::~CS60DriveCleanserAppUi()
	{
	iEngine->DeleteExistingFolder();
	if (iAppView)
		{
		RemoveFromStack( iAppView );
		delete iAppView;
		iAppView = NULL;
		}

	if ( iEngine )
		{
		delete iEngine;
		iEngine = NULL;
		}
	}

void CS60DriveCleanserAppUi::HandleCommandL( TInt aCommand )
	{
	switch (aCommand)
		{
		case EEikCmdExit:
		case EAknSoftkeyExit:
			{
			iEngine->DeleteExistingFolder();
			Exit();
			}
			break;
		case ECommandFullClean:
			{
			InitVolumeInfo();
			iEngine->FullClean();
			}
			break;
		case ECommandPartialClean:
			{
			InitVolumeInfo();
			iEngine->PartialClean();
			}
			break;
		case EInformFriend:
			{
			TInt OkPressed = iAppView->ShowSendSMSDialog();
			if( OkPressed )
				{
				iAppView->TellFriend();
				}
			}
			break;
		case ECommandHelp:
			{
			CArrayFix<TCoeHelpContext>* buf = CCoeAppUi::AppHelpContextL();
			HlpLauncher::LaunchHelpApplicationL(iEikonEnv->WsSession(), buf);
			}
			break;
		case ECommandAbout:
			{
			CAknMessageQueryDialog* dlg = new (ELeave) CAknMessageQueryDialog();
			dlg->PrepareLC( R_ABOUT_QUERY_DIALOG );
			HBufC* title = iEikonEnv->AllocReadResourceLC( R_ABOUT_DIALOG_TITLE );
			dlg->QueryHeading()->SetTextL( *title );
			CleanupStack::PopAndDestroy();
			HBufC* msg = iEikonEnv->AllocReadResourceLC( R_ABOUT_DIALOG_TEXT );
			dlg->SetMessageTextL( *msg );
			CleanupStack::PopAndDestroy();
			dlg->RunLD();
			}
			break;
		default:
			Panic( ES60DriveCleanserUi );
			break;
		}
	}

void CS60DriveCleanserAppUi::HandleStatusPaneSizeChange()
	{
	iAppView->SetRect( ClientRect() );
	}

void CS60DriveCleanserAppUi::InitVolumeInfo()
	{
	TInt driveId = iAppView->GetDriveId();
	iEngine->SetDriveId( driveId );
	}

CArrayFix<TCoeHelpContext>* CS60DriveCleanserAppUi::HelpContextL() const
	{
	CArrayFixFlat<TCoeHelpContext>* array = new (ELeave) CArrayFixFlat<
			TCoeHelpContext> (1);
	CleanupStack::PushL(array);
	array->AppendL(TCoeHelpContext(KUidS60DriveCleanserApp, KConfigure));
	CleanupStack::Pop(array);
	return array;
	}
