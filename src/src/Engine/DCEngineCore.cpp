#include <f32file.h>
#include <COEMAIN.H>
#include <aknnotewrappers.h>
#include <BAUTILS.H>
#include <sysutil.h>

#include "DCEngineCore.h"

CDCEngineCore::CDCEngineCore()
	{
	}

CDCEngineCore::~CDCEngineCore()
	{
	if( iCheckBattery )
		{
		delete iCheckBattery;
		iCheckBattery = NULL;
		}

	if ( iCheckSms )
		{
		delete iCheckSms;
		iCheckSms = NULL;
		}

	if ( iChargerStatus )
		{
		delete iChargerStatus;
		iChargerStatus = NULL;
		}
	}

CDCEngineCore* CDCEngineCore::NewLC()
	{
	CDCEngineCore* self = new (ELeave) CDCEngineCore;
	CleanupStack::PushL( self );
	self->ConstructL();
	return self;
	}

CDCEngineCore* CDCEngineCore::NewL()
	{
	CDCEngineCore* self = CDCEngineCore::NewLC();
	CleanupStack::Pop();
	return self;
	}

void CDCEngineCore::ConstructL()
	{
	iIsCharger = EFalse;
	iCheckBattery = CDCEngineCheckBattery::NewL( *this );
	iCheckSms = CDCEngineCheckSMS::NewL( this );
	iChargerStatus = CDCEngineChargerStatusCheck::NewL (*this);
	iMaxFileSize = 0;
	}

void CDCEngineCore::WriteFile( const TDesC& aFile )
	{
	TBuf8<KMaxByte> bufToWrite;
	TInt maxByte( KMaxByte );
	TInt64 byteWritten( 0 );
	RFile file;
	RFs fs;

	User::LeaveIfError( fs.Connect() );
	User::LeaveIfError( file.Replace( fs, aFile, EFileWrite ) );

	if( iMaxFileSize < KMaxByte )
		{
		maxByte = iMaxFileSize;
		}

	while( byteWritten < iMaxFileSize )
		{
		TInt64 remainSpace = iMaxFileSize - byteWritten;
		if( remainSpace >= maxByte )
			{
			bufToWrite.FillZ( maxByte );
			TRAPD( err, file.Write( bufToWrite ) );
			byteWritten += maxByte;
			}
		else
			{
			bufToWrite.SetLength( remainSpace );
			bufToWrite.FillZ( remainSpace );
			TRAPD( err, file.Write( bufToWrite ) );
			byteWritten += remainSpace;
			}
		}
	
	file.Flush();
	file.Close();
	fs.Close();
	}

void CDCEngineCore::MaximumFileSize( TInt aDriveId )
	{
	TInt64 freeSpace = DriveFreeSpace( aDriveId );
	iMaxFileSize = freeSpace / KMaximumFile;
	if( freeSpace < KMaximumFile )
		{
		iMaxFileSize = freeSpace;
		}
	}

TVolumeInfo CDCEngineCore::VolumeInfo( TInt aDriveId )
	{
	RFs fs;
	User::LeaveIfError( fs.Connect() );

	TVolumeInfo volumeInfo;
	fs.Volume( volumeInfo, aDriveId );

	fs.Close();
	return volumeInfo;
	}

TInt64 CDCEngineCore::DriveTotalSpace( TInt aDriveId )
	{
	TInt64 totalSpace;
	TVolumeInfo volumeInfo = VolumeInfo( aDriveId );
	totalSpace = volumeInfo.iSize;
	return totalSpace;
	}

TInt64 CDCEngineCore::DriveFreeSpace( TInt aDriveId )
	{
	TInt64 freeSpace;
	TVolumeInfo volumeInfo = VolumeInfo( aDriveId );
	freeSpace = volumeInfo.iFree;
	return freeSpace;
	}

void CDCEngineCore::DeleteFolder( TDesC& aFolder )
	{
	RFs fs;
	User::LeaveIfError( fs.Connect() );

	if( BaflUtils::FolderExists( fs, aFolder ) )
		{
		CFileMan* fileMan=CFileMan::NewL(fs);
		CleanupStack::PushL( fileMan );
		fileMan->RmDir( aFolder );
		CleanupStack::PopAndDestroy( fileMan );
		}

	fs.Close();
	}

void CDCEngineCore::DeleteAllFilesAndFolder( RFs aFs, const TDesC& aPath )
	{
	iDirScan = CDirScan::NewLC( aFs );
	iDirScan->SetScanDataL( aPath,
		KEntryAttDir|KEntryAttMatchExclusive,
		ESortNone, CDirScan::EScanDownTree );

	while(1)
		{
		CDir* dir = NULL;
		TRAPD(error, iDirScan->NextL( dir ));
		if( error || !dir )
			break;
		delete dir;
		ScanDirectoryL(aFs, iDirScan->FullPath());
		};
	CleanupStack::PopAndDestroy(iDirScan);
	}

void CDCEngineCore::ScanDirectoryL( RFs& aFs,const TDesC& aDir )
	{
	CDir* File_list(NULL);
	TParse parse;
	TPtrC spec(parse.FullName());
	if(KErrNone == CCoeEnv::Static()->FsSession().GetDir(aDir,
	KEntryAttMaskSupported, EDirsFirst, File_list))
		{
			if( File_list )
			{
			TRAPD( err, RecursiveDeleteFileL( aDir, CFileMan::ERecurse ));
			}
			delete File_list;
			File_list = NULL;
		}
	RecursiveDeleteFolderL( aFs , aDir );
	}

TBool CDCEngineCore::RecursiveDeleteFileL( const TDesC &aFolderPath,
															TUint aSwitch )
	{
	RFs fs;
	User::LeaveIfError( fs.Connect());
	CFileMan* fileMan=CFileMan::NewL(fs);
	CleanupStack::PushL(fileMan);
	TInt err=fileMan->Delete(aFolderPath, aSwitch);
	CleanupStack::PopAndDestroy(fileMan);
	fs.Close();
	if(err==KErrNone)
		{
		return ETrue;
		}
	else
		{
		return EFalse;
		}
	}

void CDCEngineCore::RecursiveDeleteFolderL( RFs& aFs, const TDesC& aDir )
	{
	TInt rmError;
	CFileMan* fileMan=CFileMan::NewL(aFs);
	CleanupStack::PushL(fileMan);
	rmError = fileMan->RmDir( aDir );
	CleanupStack::PopAndDestroy(fileMan);
	}

void CDCEngineCore::BatteryChargeLevelL( TUint aChargeStatus,
				CTelephony::TBatteryStatus /*aStatus*/ )
	{
	iChargeLevel = aChargeStatus;
	}

TUint CDCEngineCore::GetChargeLevel()
	{
	return iChargeLevel;
	}

void CDCEngineCore::SMSReceived()
	{
	CAknInformationNote* informationNote;
	informationNote = new ( ELeave ) CAknInformationNote;
	informationNote->ExecuteLD( KMsgForSmsReceived );
	}

void CDCEngineCore::ChargerConnected()
	{
	iIsCharger = ETrue;
	}

void CDCEngineCore::ChargerIsNotConnected()
	{
	iIsCharger = EFalse;
	}

TBool CDCEngineCore::GetChragerStatus()
	{
	return iIsCharger;
	}
