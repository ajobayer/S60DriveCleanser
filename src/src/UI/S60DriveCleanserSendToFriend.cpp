#include <CPbkContactEngine.h>
#include <f32file.h>
#include <eikenv.h>
#include <aknnotewrappers.h>
#include <cpbksingleentryfetchdlg.h>
#include <CPbkMultipleEntryFetchDlg.h>
#include <cpbksmsaddressselect.h>
#include <stringloader.h>
#include <cpbkcontactitem.h>
#include <cpbkcontactiter.h>
#include <rpbkviewresourcefile.h>
#include <cpbkcontacteditordlg.h>
#include <aknview.h>

#include "S60DriveCleanserSMSSender.h"
#include <S60DriveCleanser_0x20027916.rsg>
#include "S60DriveCleanserSendToFriend.h"

CS60DriveCleanserSendToFriend::CS60DriveCleanserSendToFriend()
:iPhonebookResource( *iCoeEnv )
	{
	}

CS60DriveCleanserSendToFriend::~CS60DriveCleanserSendToFriend()
	{
	if(iArray)
		{
		iArray->Reset();
		delete iArray;
		}
	}

CS60DriveCleanserSendToFriend* CS60DriveCleanserSendToFriend::NewL()
	{
	CS60DriveCleanserSendToFriend* self = 
									CS60DriveCleanserSendToFriend::NewLC();
	CleanupStack::Pop();
	return self;
	}

CS60DriveCleanserSendToFriend* CS60DriveCleanserSendToFriend::NewLC()
	{
	CS60DriveCleanserSendToFriend* self = 
							new (ELeave) CS60DriveCleanserSendToFriend();
	CleanupStack::PushL( self );
	self->ConstructL();
	return self;
	}

void CS60DriveCleanserSendToFriend::ConstructL()
	{
	}

void CS60DriveCleanserSendToFriend::FindContact()
	{
	iMessage.SetLength( 0 );
	iMessage.Copy( KMessageText );
	TBuf<KPhoneMaxSize> phoneNumber;
	iSelectedOption = 0;
	TInt listQueryDlg = 0;
	listQueryDlg = ShowListQueryDialog();
    if( listQueryDlg )
	    {
	    phoneNumber.SetLength( 0 );
		if( iSelectedOption == 0 )
		    {
		    TBuf< KPhoneMaxSize > phoneNumber;
		    phoneNumber.SetLength( 0 );
		    phoneNumber.Copy( DisplayPhoneNumbersL() );
		    if( phoneNumber.Length() != 0 )
		    	{
		    	SendSMSToFriend( phoneNumber );
		    	}
		    }
		else
		    {
		    CAknTextQueryDialog* NewContactDlg = 
									CAknTextQueryDialog::NewL( phoneNumber );
		    if(NewContactDlg->ExecuteLD( R_PHONE_NUMBER_QUERY_FOR_OK_CANCEL ))
		    	{
		    	SendSMSToFriend( phoneNumber );
		       	}
		    }
	    }
	}

void CS60DriveCleanserSendToFriend::SendSMSToFriend( const TDesC& aPhoneNumber )
	{
	CS60DriveCleanserSMSSender* iSmsSend = NULL;
	iSmsSend = CS60DriveCleanserSMSSender::NewL();
	CleanupStack::PushL( iSmsSend );
	iSmsSend->SendSMSL( aPhoneNumber, iMessage );
	CleanupStack::PopAndDestroy( iSmsSend );
	}

TInt CS60DriveCleanserSendToFriend::ShowListQueryDialog()
	{
	TInt viewItem = 2;
	iArray = new (ELeave) CDesCArrayFlat( viewItem );
	iArray->AppendL( _L( "Search contact" ) );
	iArray->AppendL( _L( "New contact" ) );
	CAknListQueryDialog* ContactDlg = 
							new(ELeave) CAknListQueryDialog( &iSelectedOption );
	ContactDlg->PrepareLC( R_UPDATE_QUERY_DIALOG );
	ContactDlg->SetItemTextArray( iArray );
	ContactDlg->SetOwnershipType( ELbmDoesNotOwnItemArray );
	return( ContactDlg->RunLD() );
	}

TBuf<KPhoneMaxSize> CS60DriveCleanserSendToFriend::DisplayPhoneNumbersL()
    {
    TBuf<KPhoneMaxSize> phoneNumber;
	phoneNumber.SetLength( 0 );
	iPhonebookResource.OpenL();
	CPbkContactEngine* iPbkEngine = CPbkContactEngine::NewL();
	CleanupStack::PushL( iPbkEngine );
	CPbkSingleEntryFetchDlg::TParams params;
	CPbkSingleEntryFetchDlg* fetchDlg = 
									CPbkSingleEntryFetchDlg::NewL( params );
	fetchDlg->SetMopParent( this );
	TInt okPressed = fetchDlg->ExecuteLD();
	if( okPressed != 0 )
		{
		TContactItemId contactId = params.iSelectedEntry;
		CPbkContactItem* pbkItem = 
								iPbkEngine->ReadMinimalContactLC( contactId );
		const TPbkContactItemField* selectedField = NULL;
		CPbkSmsAddressSelect::TParams addParams( *pbkItem );
		CPbkSmsAddressSelect* selectDlg = new(ELeave) CPbkSmsAddressSelect();
		if( selectDlg->ExecuteLD( addParams ) )
			{
			const TPbkContactItemField *selectedField = 
												addParams.SelectedField();
			phoneNumber.Copy( selectedField->Text() );
			}
		CleanupStack::PopAndDestroy( pbkItem );
		}
	iPhonebookResource.Close();
	CleanupStack::PopAndDestroy( iPbkEngine );
	return phoneNumber;
    }
