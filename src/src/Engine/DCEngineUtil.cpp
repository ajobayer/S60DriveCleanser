#include <pathinfo.h>
#include <e32std.h>
#include <e32base.h>
#include <BADESCA.H>
#include <f32file.h>
#include <sysutil.h>

#include "DCEngineUtil.h"
#include "DCConstants.h"

CDCEngineUtil::CDCEngineUtil()
	{
	}

CDCEngineUtil::~CDCEngineUtil()
	{
	}

CDCEngineUtil* CDCEngineUtil::NewLC()
	{
	CDCEngineUtil* self = new (ELeave) CDCEngineUtil;
	CleanupStack::PushL( self );
	self->ConstructL();
	return self;
	}

CDCEngineUtil* CDCEngineUtil::NewL()
	{
	CDCEngineUtil* self = CDCEngineUtil::NewLC();
	CleanupStack::Pop();
	return self;
	}

void CDCEngineUtil::ConstructL()
	{
	}

TBuf<KMaxDrives> CDCEngineUtil::RetriveDriveList()
	{
	RFs fsSession;
	TDriveList drivelist;
	TInt driveNumber;
	TChar driveLetter;
	TBuf<KMaxDrives> driveName;

	User::LeaveIfError( fsSession.Connect() );
	User::LeaveIfError( fsSession.DriveList( drivelist ) );

	TVolumeInfo volumeInfo;

	for ( driveNumber = EDriveA; driveNumber <= EDriveZ; driveNumber++ )
		{
		if ( drivelist[ driveNumber ] )
			{
			User::LeaveIfError( fsSession.DriveToChar( driveNumber,
														driveLetter ) );
			if( ( driveNumber != EDriveD ) && ( driveNumber != EDriveZ ) )
				{
				if( KErrNone == fsSession.Volume( volumeInfo ,
			            					driveNumber ) )
					{
					driveName.Append( driveLetter );
					}
				}
			}
		}
	fsSession.Close();
	return driveName;
	}

TBuf<KMaxFileName> CDCEngineUtil::DirectoryRootPath( TInt aDriveId )
	{
	TFileName rootDir;
	
	switch( aDriveId )
		{
		case EDriveC:
			{
			rootDir.Copy( KDriveC );
			}
			break;
		case EDriveE:
			{
			rootDir.Copy(PathInfo::MemoryCardRootPath() );
			}
			break;
		default:
			break;
		}

	return rootDir;
	}

TBuf<KMaxFileName> CDCEngineUtil::DirectoryPath( TInt aDriveId )
	{
	TFileName filePath = DirectoryRootPath( aDriveId );
	filePath.Append( KWriteDir );
	return filePath;
	}

void CDCEngineUtil::MakeDirectory( TDesC& aDir )
	{
	RFs fs;
	User::LeaveIfError( fs.Connect() );
	fs.MkDir( aDir );
	fs.Close();
	}

void CDCEngineUtil::FactoryReset()
	{
	const TUid uid = { 0x20027916 };
	SysStartup::ShutdownAndRestart( uid, ESWRestoreFactorySetDeep );
	}

