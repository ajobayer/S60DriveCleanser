#ifndef DCENGINECHECKBATTERY_H
#define DCENGINECHECKBATTERY_H

#include <e32base.h>
#include <e32std.h>
#include <Etel3rdParty.h>
#include <COEMAIN.H>

#include "MDCEngineBatteryObserver.h"

class MBatteryObserver;

class CDCEngineCheckBattery : public CActive
	{
	public:
		~CDCEngineCheckBattery();
		static CDCEngineCheckBattery* NewL( MBatteryObserver& aObserver);
		static CDCEngineCheckBattery* NewLC( MBatteryObserver& aObserver);

	private:
		CDCEngineCheckBattery( MBatteryObserver& aObserver );
		void ConstructL();

	private:
		void GetBatteryInfo();
		void RunL();
		void DoCancel();

	private:
		MBatteryObserver&               iObserver;
		CTelephony*                     iTelephony;
		CTelephony::TBatteryInfoV1      iBatteryInfoV1;
		CTelephony::TBatteryInfoV1Pckg  iBatteryInfoV1Pckg;
		TBool                           iGettingBattery;
		
	};

#endif // DCENGINECHECKBATTERY_H
