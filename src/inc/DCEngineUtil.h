#ifndef DCENGINEUTIL_H
#define DCENGINEUTIL_H

#include "DCFactoryReset.h"

class CDCEngineUtil : public CBase
	{
	public:
		~CDCEngineUtil();
		static CDCEngineUtil* NewL();
		static CDCEngineUtil* NewLC();

	public:
		TBuf<KMaxDrives> RetriveDriveList();
		void MakeDirectory( TDesC& aDir );
		TBuf<KMaxFileName> DirectoryRootPath( TInt aDriveId );
		TBuf<KMaxFileName> DirectoryPath( TInt aDriveId );
		void FactoryReset();

	private:
		CDCEngineUtil();
		void ConstructL();

	};

#endif
