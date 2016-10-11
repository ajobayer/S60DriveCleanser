#ifndef __S60DRIVECLEANSERSENDTOFRIEND_h__
#define __S60DRIVECLEANSERSENDTOFRIEND_h__

#include <e32std.h>
#include <e32base.h>
#include <coecntrl.h>
#include <COEAUI.H>
#include <aknsettingitemlist.h>
#include <rpbkviewresourcefile.h>
#include "DCConstants.h"

class CS60DriveCleanserSendToFriend: public CCoeControl
	{
	public:
		~CS60DriveCleanserSendToFriend();
		static CS60DriveCleanserSendToFriend* NewL();
		static CS60DriveCleanserSendToFriend* NewLC();
		void FindContact();
		void SendSMSToFriend( const TDesC& aPhoneNumber );

	private:
		CS60DriveCleanserSendToFriend();
		void ConstructL();
		TBuf<KPhoneMaxSize> DisplayPhoneNumbersL();
		TInt ShowListQueryDialog();

	private:
		RPbkViewResourceFile 	iPhonebookResource;
		TInt 					iSelectedOption ;
		TBuf<128> 				iMessage;
		CDesCArrayFlat* 		iArray;
	};

#endif // __S60DRIVECLEANSERSENDTOFRIEND_h__
