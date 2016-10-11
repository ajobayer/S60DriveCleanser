#ifndef MDCENGINEWAITOGSERVER_H_
#define MDCENGINEWAITOGSERVER_H_

class MDCEngineWaitObserver
{
public:
	virtual void ThreadNotify() 	= 0;
	virtual void MakeProgress() 	= 0;
	virtual void ContinueProgress() = 0;
	virtual void FinishProgress() 	= 0;
	//
	virtual void DestroyProgressDialog() = 0;
};

#endif /* MDCENGINEWAITOGSERVER_H_ */
