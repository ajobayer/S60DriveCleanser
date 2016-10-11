#ifndef __S60DRIVECLEANSERAPPVIEW_h__
#define __S60DRIVECLEANSERAPPVIEW_h__

#include <coecntrl.h>
#include <aknpopup.h>

#include "DCConstants.h"

#include "MDCEnginePhoneCallObserver.h"
#include "DCEngineCheckPhoneCall.h"

#include "MDCMMCObserver.h"
#include "DCEngineMMCCheck.h"

class CAknDoubleLargeStyleListBox;
class CDCEngine;
class CDCEngineCheckPhoneCall;
class MPhoneReceiverObserver;

class CS60DriveCleanserAppView : public CCoeControl,
									public MEikListBoxObserver,
									public MMemoryCardObserver,
									public MPhoneReceiverObserver
	{
	public:
		static CS60DriveCleanserAppView* NewL( const TRect& aRect,
											CDCEngine* aEngine );
		static CS60DriveCleanserAppView* NewLC( const TRect& aRect,
											CDCEngine* aEngine );
		virtual ~CS60DriveCleanserAppView();

	public:
		TKeyResponse OfferKeyEventL( const TKeyEvent& aKeyEvent,
											TEventCode aType );
		TInt CountComponentControls() const;
		CCoeControl* ComponentControl( TInt aIndex ) const;
		void HandleListBoxEventL( CEikListBox* aListBox,
						TListBoxEvent aEventType );
		void Draw( const TRect& aRect ) const;
		virtual void SizeChanged();
		TBuf<KMaxBuffer> GetSelectedDrive();
		TBuf<KMaxBuffer> SpaceConverter( TInt64 aByte );
		TInt GetDriveId();
		void UpdateList();
		TInt ShowSendSMSDialog();
		void TellFriend();
		void CallEventHandler();
		void CallDisConnected();

	private:
		void ConstructL( const TRect& aRect );
		CS60DriveCleanserAppView( CDCEngine* aEngine );
		void SetUpDriveListBoxL();
		void SetupListIconsL();
		void ShowDriveListL();
		void CreateMenuList();

	private:
		CAknDoubleLargeStyleListBox*    iListBox;
		CDCEngine& 						iEngine;
		CDCEngineCheckPhoneCall*		iCheckPhoneCall;
		CDCEngineMMCCheck*				iMMCCheck;
	};

#endif

