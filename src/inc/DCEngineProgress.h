#ifndef DCENGINEPROGRESS_H_
#define DCENGINEPROGRESS_H_

#include <eikprogi.h>
#include <AknProgressDialog.h>

class CDCEngineActiveObject;
 
class CDCEngineProgress : public CBase, public MProgressDialogCallback
	{	
	public: 
	   ~CDCEngineProgress();	   
		static CDCEngineProgress* NewL( CDCEngineActiveObject& aActiveObj );
		static CDCEngineProgress* NewLC( CDCEngineActiveObject& aActiveObj );
		
	private:
		CDCEngineProgress( CDCEngineActiveObject& aActiveObj );
		void ConstructL();
		
	public:
	   void StartProgressNoteL( TInt aFinalValue, TInt aResId )	;
	   void UpdateProcessL( TInt aProgress, const TDesC& aProgressText );
	   void FinishProcessL();
		
	protected:
	   void DialogDismissedL( TInt aReason );
	 
	private:	
	   CAknProgressDialog*  	iProgressDialog;
	   CEikProgressInfo*    	iProgressInfo;
	   CDCEngineActiveObject& 	iActiveObj;
	};

#endif /* DCENGINEPROGRESS_H_ */
