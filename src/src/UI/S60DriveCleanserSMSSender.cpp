#include <aknquerydialog.h> 
#include <rsendas.h>
#include <rsendasmessage.h>
#include <smut.h>

#include "S60DriveCleanserSMSSender.h"

CS60DriveCleanserSMSSender::CS60DriveCleanserSMSSender()
	{
	
	}

CS60DriveCleanserSMSSender::~CS60DriveCleanserSMSSender()
	{
	
	}

CS60DriveCleanserSMSSender* CS60DriveCleanserSMSSender::NewL()
	{
	CS60DriveCleanserSMSSender* self = CS60DriveCleanserSMSSender::NewLC();
	CleanupStack::Pop();
	return self;
	}

CS60DriveCleanserSMSSender* CS60DriveCleanserSMSSender::NewLC()
	{
	CS60DriveCleanserSMSSender* self = 
									new (ELeave) CS60DriveCleanserSMSSender();
	CleanupStack::PushL( self );
	self->ConstructL();
	return self;
	}

void CS60DriveCleanserSMSSender::ConstructL()
	{
	}

void CS60DriveCleanserSMSSender::SendSMSL( const TDesC& aSenderAddr, 
														const TDesC& aMsg )
	{
	RSendAs sendAs;
	TInt err = sendAs.Connect();
	if ( err ) 
		{
		return;
		}
	CleanupClosePushL( sendAs );

	RSendAsMessage sendAsMessage;
	sendAsMessage.CreateL( sendAs, KUidMsgTypeSMS );
	CleanupClosePushL( sendAsMessage );

	sendAsMessage.AddRecipientL( aSenderAddr, 
								RSendAsMessage::ESendAsRecipientTo );
	sendAsMessage.SetBodyTextL( aMsg );

	sendAsMessage.SendMessageAndCloseL();
	CleanupStack::Pop(); 
	CleanupStack::PopAndDestroy();  
   }



