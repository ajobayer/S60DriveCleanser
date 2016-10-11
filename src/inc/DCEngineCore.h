#ifndef DCENGINECORE_H
#define DCENGINECORE_H

#include <e32std.h>
#include <e32base.h>
#include <f32file.h>
#include <S32FILE.H>

#include "DCConstants.h"
#include "MDCEngineBatteryObserver.h"
#include "DCEngineCheckBattery.h"
#include "MDCEngineSmsObserver.h"
#include "DCEngineCheckSMS.h"
#include "DCEngineChargerStatusCheck.h"
#include "MDCChargerObserver.h"

class CDCEngineCheckSMS;
class DCEngineChargerStatusCheck;

class CDCEngineCore : public CBase,
					  public MBatteryObserver,
					  public MChargerObserver,
					  public MSMSEngineObserver
	{
	public:
		~CDCEngineCore();
		static CDCEngineCore* NewL();
		static CDCEngineCore* NewLC();

	public:
		void WriteFile( const TDesC& aFile );
		void MaximumFileSize( TInt aDriveId );
		TVolumeInfo VolumeInfo( TInt aDriveId );
		TInt64 DriveTotalSpace( TInt aDriveId );
		TInt64 DriveFreeSpace( TInt aDriveId );
		void DeleteFolder( TDesC& aFolder );
		void DeleteAllFilesAndFolder( RFs, const TDesC& );
		void ScanDirectoryL( RFs& aFs, const TDesC& aDir );
		TBool RecursiveDeleteFileL( const TDesC &aFolderPath, TUint aSwitch );
		void RecursiveDeleteFolderL( RFs& aFs, const TDesC& aDir );
		TUint GetChargeLevel();
		void BatteryChargeLevelL( TUint aChargeStatus,
		CTelephony::TBatteryStatus aStatus );
		void SMSReceived();
		void ChargerConnected();
		TBool GetChragerStatus();
		void ChargerIsNotConnected();

	private:
		CDCEngineCore();
		void ConstructL();

	private:
		CDirScan* 						iDirScan;
		TUint							iChargeLevel;
		CDCEngineCheckBattery*			iCheckBattery;
		CDCEngineCheckSMS*				iCheckSms;
		TInt64							iMaxFileSize;
		CDCEngineChargerStatusCheck*	iChargerStatus;
		TBool							iIsCharger;
	};

#endif
