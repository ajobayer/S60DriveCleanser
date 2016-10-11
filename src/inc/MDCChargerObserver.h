/*
 * MDCChargerObserver.h
 *
 *  Created on: Jul 15, 2009
 *      Author: Administrator
 */

#ifndef MDCCHARGEROBSERVER_H_
#define MDCCHARGEROBSERVER_H_

class MChargerObserver
	{
public:
	
	virtual void ChargerConnected() = 0;
	virtual void ChargerIsNotConnected() = 0;
	};

#endif /* MDCCHARGEROBSERVER_H_ */
