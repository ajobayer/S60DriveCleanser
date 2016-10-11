#ifndef __S60DRIVECLEANSERSMSSENDER_h__
#define __S60DRIVECLEANSERSMSSENDER_h__

#include <e32base.h>
#include <in_sock.h>

class CS60DriveCleanserSMSSender : public CBase
	{
	public:
		static CS60DriveCleanserSMSSender* NewL();
		static CS60DriveCleanserSMSSender* NewLC();
		virtual ~CS60DriveCleanserSMSSender();
		
	public:
		void SendSMSL( const TDesC& aSenderAddr, const TDesC& aMsg );	
		
	private:		
		CS60DriveCleanserSMSSender();
		void ConstructL();		
	};

#endif // __S60DRIVECLEANSERSMSSENDER_h__
