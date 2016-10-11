#ifndef MDCENGINEBATTERYOBSERVER_H_
#define MDCENGINEBATTERYOBSERVER_H_

#include <Etel3rdParty.h>

class MBatteryObserver
	{
	public:
		virtual void BatteryChargeLevelL( TUint aChargeStatus,
				CTelephony::TBatteryStatus aStatus ) = 0;
	};

#endif /* MDCENGINEBATTERYOBSERVER_H_ */
