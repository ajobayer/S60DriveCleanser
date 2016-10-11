#include <aknlists.h>
#include <BARSREAD.H>
#include <gulicon.h>
#include <akniconarray.h>

#include <S60DriveCleanser_svg.mbg>
#include <S60DriveCleanser_0x20027916.rsg>
#include "S60DriveCleanserAppView.h"
#include "DCEngine.h"
#include "S60DriveCleanserSendToFriend.h"

CS60DriveCleanserAppView* CS60DriveCleanserAppView::NewL( const TRect& aRect,
														CDCEngine* aEngine )
	{
	CS60DriveCleanserAppView* self = CS60DriveCleanserAppView::NewLC( aRect,
																aEngine );
	CleanupStack::Pop(self);
	return self;
	}


CS60DriveCleanserAppView* CS60DriveCleanserAppView::NewLC( const TRect& aRect,
														CDCEngine* aEngine )
	{
	CS60DriveCleanserAppView* self =
						new ( ELeave ) CS60DriveCleanserAppView( aEngine );
	CleanupStack::PushL( self );
	self->ConstructL( aRect );
	return self;
	}


void CS60DriveCleanserAppView::ConstructL( const TRect& aRect )
	{
	CreateWindowL();
	iEngine.DeleteExistingFolder();
	SetUpDriveListBoxL();
	ShowDriveListL();

	SetRect( aRect );
	ActivateL();

	iCheckPhoneCall = CDCEngineCheckPhoneCall::NewL( *this );
	//iMMCCheck = CDCEngineMMCCheck::NewL( *this );
	}

CS60DriveCleanserAppView::CS60DriveCleanserAppView( CDCEngine* aEngine )
: iEngine( *aEngine )
	{
	}

CS60DriveCleanserAppView::~CS60DriveCleanserAppView()
	{
	delete iListBox;
	if( iCheckPhoneCall )
		{
		delete iCheckPhoneCall;
		iCheckPhoneCall = NULL;
		}

	/*if( iMMCCheck )
		{
		delete iMMCCheck;
		iMMCCheck = NULL;
		}*/
	}

void CS60DriveCleanserAppView::SetUpDriveListBoxL()
    {
    iListBox = new ( ELeave ) CAknDoubleLargeStyleListBox();
	iListBox->SetContainerWindowL( *this );
	TResourceReader reader;
	iEikonEnv->CreateResourceReaderLC( reader, R_DRIVE_VIEW_LISTBOX );
	iListBox->ConstructFromResourceL( reader );
	CleanupStack::PopAndDestroy();
	iListBox->SetListBoxObserver( this );
	SetupListIconsL();
	iListBox->CreateScrollBarFrameL( ETrue );
	iListBox->ScrollBarFrame()->SetScrollBarVisibilityL(
				CEikScrollBarFrame::EOff, CEikScrollBarFrame::EAuto );
    }

void CS60DriveCleanserAppView::SetupListIconsL()
	{
	TBuf<KMaxBuffer> svgFile;
	CArrayPtr< CGulIcon >* icons = new (ELeave) CAknIconArray( KTotalImage );

	svgFile.Copy( KSvgFileName );
	TFileName fileName = svgFile;
	TInt totalImage = KTotalImage;

	TSize pictureSize( 10,10 );
	TInt iconId = EMbmS60drivecleanser_svgDrive;
	CFbsBitmap* appIconBitmap = NULL;
	CFbsBitmap* appIconMask = NULL;
	for( TInt imageNumber = 0 ; imageNumber < totalImage ; imageNumber++ )
		{
		appIconBitmap = NULL;
		appIconMask = NULL;
		AknIconUtils::CreateIconL( appIconBitmap, appIconMask, fileName,
												iconId, iconId + 1 );
		AknIconUtils::SetSize( appIconBitmap, pictureSize );
		AknIconUtils::SetSize( appIconMask, pictureSize );

		CleanupStack::PushL( appIconBitmap );
		CleanupStack::PushL( appIconMask );

		icons->AppendL( CGulIcon::NewL( appIconBitmap, appIconMask ) );

		CleanupStack::Pop( 2 );
		iconId = iconId + 2;
		}

	if ( icons )
		{
		if ( iListBox->ItemDrawer()->ColumnData()->IconArray() )
			{
			iListBox->ItemDrawer()->ColumnData()->IconArray()
									  ->ResetAndDestroy();
			}
		iListBox->ItemDrawer()->ColumnData()->SetIconArray( icons );
		}
	 else
		{
		return;
		}
	iListBox->ItemDrawer()->ColumnData()->SetMarqueeParams (10, 20, 1000000, 400000);
	iListBox->ItemDrawer()->ColumnData()->EnableMarqueeL(ETrue);
	}

void CS60DriveCleanserAppView::ShowDriveListL()
	{
 	CTextListBoxModel* model = iListBox->Model();
	User::LeaveIfNull( model );
	model->SetOwnershipType( ELbmOwnsItemArray );

	CDesCArray* itemArray = static_cast< CDesCArray* >
								( model->ItemTextArray() );
	User::LeaveIfNull( itemArray );
	itemArray->Reset();

	TBuf<KMaxBuffer> freeSpace;
	TBuf<KMaxBuffer> totalSpace;
	TBuf<KMaxBuffer> usedSpace;
	TBuf<KMaxDrives> totalDrive;
	TInt driveNumber;

	RFs fsSession;
	User::LeaveIfError( fsSession.Connect() );
	totalDrive = iEngine.GetAvailableDriveList();

	for ( TInt counter = 0; counter < totalDrive.Length(); counter++ )
		{
		User::LeaveIfError( fsSession.CharToDrive( totalDrive[ counter ],
												driveNumber ) );

		TInt64 byteFreeSpace = iEngine.GetDriveFreeSpace( driveNumber );
		freeSpace = SpaceConverter( byteFreeSpace );

		TInt64 byteTotalSpace = iEngine.GetDriveTotalSpace( driveNumber );
		totalSpace = SpaceConverter( byteTotalSpace );
		TInt64 byteUsedSpace = byteTotalSpace - byteFreeSpace;
		usedSpace = SpaceConverter( byteUsedSpace );
		TBuf<KMaxBuffer> string;
		string.Format( KListFormatString, KDriveIconSlot );
		if( totalDrive[ counter ] == 'C' )
			{
			string.Append( KLabelPhoneMemory );
			}
		else if( totalDrive[ counter ] == 'E' )
			{
			string.Append( KLabelExternalMemory );
			}
		string.Append( KTab );
		string.Append( freeSpace );
		string.Append( KLabelOf );
		string.Append( totalSpace );
		string.Append( KLabelIsFree );

		itemArray->AppendL( string );
		iListBox->HandleItemAdditionL();
		string.Zero();
		}

	fsSession.Close();
	iListBox->SetCurrentItemIndex( 0 );
	iListBox->DrawNow();
	}

TBuf<KMaxBuffer> CS60DriveCleanserAppView::SpaceConverter( TInt64 aByte )
	{
	TBuf<KMaxBuffer> spaceValue;
	TReal realSpace;
	TRealFormat format( KMaxBuffer, KDecimalPlaces );

	if( float( aByte )/float( KUnit ) < 1 )
		{
		spaceValue.Num( aByte );//byte
		spaceValue.Append( KTextByte );
		}
	else
		{
		if( float( aByte )/float( ( KUnit * KUnit ) )< 1 )
			{
			realSpace = float( aByte )/float( KUnit );
			spaceValue.Num( realSpace,format );//kb
			spaceValue.Append( KTextKB );
			}
		else
			{
			if( float( aByte )/float( ( KUnit* KUnit* KUnit ) )< 1 )
				{
				realSpace = float( aByte )/float( ( KUnit*KUnit ) );
				spaceValue.Num( realSpace, format );//mb
				spaceValue.Append( KTextMB );
				}
			else
				{
				realSpace = float( aByte )/float( ( KUnit * KUnit * KUnit ) );

				spaceValue.Num( realSpace, format );//gb
				spaceValue.Append( KTextGB );
				}
			}
		}
	return spaceValue;
	}

void CS60DriveCleanserAppView::HandleListBoxEventL( CEikListBox* /*aListBox*/,
											TListBoxEvent aEventType )
	{
	switch( aEventType )
		{
		case EEventEnterKeyPressed:
			{
			CreateMenuList();
			}
			 break;
		case EEventItemClicked:
			{
			}
		    break;
		default:
		break;
		};
	}

TBuf<KMaxBuffer> CS60DriveCleanserAppView::GetSelectedDrive()
	{
	TPtrC16 itemText = iListBox->Model()->ItemTextArray()->MdcaPoint
							( iListBox->CurrentItemIndex() );
	TInt startPos = itemText.Locate( '\t' ) + 1;
	TInt endPos = itemText.LocateReverse( '\t' );
	TBuf<KMaxBuffer> driveName;
	TBuf<KMaxBuffer> driveLetter;
	driveName =  itemText.Mid( startPos, endPos - startPos );
	if( driveName == KLabelPhoneMemory )
		{
		driveLetter.Append( KDriveC );
		}

	else if( driveName == KLabelExternalMemory )
		{
		driveLetter.Append( KDriveE );
		}
	return driveLetter;
	}

TKeyResponse CS60DriveCleanserAppView::OfferKeyEventL( const TKeyEvent&
										aKeyEvent,TEventCode aType )
	{
	return ( iListBox->OfferKeyEventL( aKeyEvent, aType ) );
	}

TInt CS60DriveCleanserAppView::CountComponentControls() const
	{
	return ( 1 );
	}

CCoeControl* CS60DriveCleanserAppView::ComponentControl( TInt aIndex ) const
	{
	switch ( aIndex )
		{
		case 0:
			return ( iListBox );
		default:
			return ( NULL );
		}
	}

void CS60DriveCleanserAppView::Draw( const TRect& aRect ) const
	{
	CWindowGc& gc = SystemGc();
	gc.Clear( aRect );
	}


void CS60DriveCleanserAppView::SizeChanged()
	{
	const TPoint listPosition( 0, 0 );
	iListBox->SetExtent( listPosition, iListBox->MinimumSize() );
	}

TInt CS60DriveCleanserAppView::GetDriveId()
	{
	TBuf<KBufSize> selectedDrive = GetSelectedDrive();
	if( selectedDrive.Compare(KDriveC) == 0 )
		{
		return EDriveC;
		}
	else if( selectedDrive.Compare(KDriveE) == 0 )
		{
		return EDriveE;
		}
	return 0;
	}

void CS60DriveCleanserAppView::CreateMenuList()
	{
	CAknSinglePopupMenuStyleListBox* list =
								new(ELeave) CAknSinglePopupMenuStyleListBox;
	CleanupStack::PushL(list);
	CAknPopupList* popupList = CAknPopupList::NewL(list,
				R_AVKON_SOFTKEYS_SELECT_CANCEL, AknPopupLayouts::EMenuWindow);
	CleanupStack::PushL(popupList);
	list->ConstructL(popupList, CEikListBox::ELeftDownInViewRect);
	list->CreateScrollBarFrameL(ETrue);
	list->ScrollBarFrame()->SetScrollBarVisibilityL(CEikScrollBarFrame::EOff,
												CEikScrollBarFrame::EAuto);
	CDesCArray *itemList = new (ELeave) CDesCArrayFlat( KItemNumber );
	TBuf<100> item;
	item.Format( KListItemFormat, &KFirstItem() );
	itemList->AppendL( item );
	item.Format( KListItemFormat, &KNextItem() );
	itemList->AppendL( item );

	list->Model()->SetItemTextArray(itemList);
	list->Model()->SetOwnershipType( ELbmOwnsItemArray );
	list->ItemDrawer()->FormattedCellData()->EnableMarqueeL( ETrue );

	CleanupStack::Pop();
	TBool popupOk = popupList->ExecuteLD();
	if( popupOk )
		{
		if( list->CurrentItemIndex() == 0 )
			{
			iEngine.SetDriveId( GetDriveId() );
			iEngine.FullClean();
			}
		else
			{
			iEngine.SetDriveId( GetDriveId() );
			iEngine.PartialClean();
			}
		}
	CleanupStack::PopAndDestroy();
	}

void CS60DriveCleanserAppView::UpdateList()
	{
//	TBuf<KBufSize> selectedDrive = GetSelectedDrive();
//	//if( selectedDrive[0] == 'E' )
//	if( selectedDrive.Compare( KDriveE ) == 0 )
//		{
//		iEngine.StopProgress();
//		}
//	//iEngine.DeleteExistingFolder();
//	iListBox->Reset();
//	ShowDriveListL();

	}

TInt CS60DriveCleanserAppView::ShowSendSMSDialog()
 	{
 	CAknMessageQueryDialog* dlg =
								new (ELeave) CAknMessageQueryDialog();
	dlg->PrepareLC( R_ABOUT_SMS_QUERY_DIALOG );
	HBufC* title = iEikonEnv->AllocReadResourceLC(
									R_SEND_FRIEND_ABOUT_SMS_TITLE );
	dlg->QueryHeading()->SetTextL( *title );
	CleanupStack::PopAndDestroy();
	HBufC* msg = iEikonEnv->AllocReadResourceLC
										( R_SEND_FRIEND_QUERY_DIALOG_TEXT );
	dlg->SetMessageTextL( *msg );
	CleanupStack::PopAndDestroy();
	return(dlg->RunLD());
 	}

void CS60DriveCleanserAppView::TellFriend()
	{
	CS60DriveCleanserSendToFriend* contactSetting = NULL;
	contactSetting = CS60DriveCleanserSendToFriend::NewL();
	contactSetting->FindContact();
	if( contactSetting )
		{
		delete contactSetting;
		contactSetting = NULL;
		}
	}

void CS60DriveCleanserAppView::CallEventHandler()
	{
	iEngine.PauseDialog();
	}

void CS60DriveCleanserAppView::CallDisConnected()
	{
	iEngine.ThreadCall();
	}
