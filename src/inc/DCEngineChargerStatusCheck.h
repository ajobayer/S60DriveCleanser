/*
 ============================================================================
 Name		: CDCEngineChargerStatusCheck.h
 Author	  :
 Version	 : 1.0
 Copyright   : Your copyright notice
 Description : CCDCEngineChargerStatusCheck declaration
 ============================================================================
 */

#ifndef CDCENGINECHARGERSTATUSCHECK_H
#define CDCENGINECHARGERSTATUSCHECK_H

#include <e32base.h>
#include <e32std.h>
#include <Etel3rdParty.h>
#include <COEMAIN.H>

#include "MDCChargerObserver.h"

class MChargerObserver;

class CDCEngineChargerStatusCheck : public CActive
	{
public:

	~CDCEngineChargerStatusCheck();


	static CDCEngineChargerStatusCheck* NewL( MChargerObserver& aObserver );


	static CDCEngineChargerStatusCheck* NewLC( MChargerObserver& aObserver );

public:
	
	void DetectCharger();
	void ChargerListener();

private:

	CDCEngineChargerStatusCheck( MChargerObserver& aObserver );
	void ConstructL();

private:

	void RunL();
	void DoCancel();

private:
	MChargerObserver&				iObserver;
	CTelephony* 					iTelephony;
	CTelephony::TIndicatorV1 		iIndicatorV1;
	CTelephony::TIndicatorV1Pckg	iIndicatorV1Pckg;

	};

#endif // CDCENGINECHARGERSTATUSCHECK_H
