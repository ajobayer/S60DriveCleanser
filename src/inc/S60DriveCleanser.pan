#ifndef __S60DRIVECLEANSER_PAN__
#define __S60DRIVECLEANSER_PAN__

/** S60DriveCleanser application panic codes */
enum TS60DriveCleanserPanics
	{
	ES60DriveCleanserUi = 1
	// add further panics here
	};

inline void Panic(TS60DriveCleanserPanics aReason)
	{
	_LIT(applicationName, "S60DriveCleanser");
	User::Panic(applicationName, aReason);
	}

#endif // __S60DRIVECLEANER_PAN__
