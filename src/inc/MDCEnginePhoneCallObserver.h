/*
 * MDCEnginePhoneCallObserver.h
 *
 *  Created on: Jul 17, 2009
 *      Author: Administrator
 */

#ifndef MDCENGINEPHONECALLOBSERVER_H_
#define MDCENGINEPHONECALLOBSERVER_H_

class MPhoneReceiverObserver
{
public:
	/*virtual void CallRinging() = 0;
	virtual void CallAnswered() = 0;
	virtual void DialingCall() = 0;
	virtual void CallConnecting() = 0;
	virtual void CallConnected() = 0;
	virtual void DialingCall() 		= 0;*/
	virtual void CallDisConnected() = 0;
	virtual void CallEventHandler() = 0;
 };

#endif /* MDCENGINEPHONECALLOBSERVER_H_ */
