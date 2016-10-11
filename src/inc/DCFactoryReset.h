#ifndef DCFACTORYRESET_H_
#define DCFACTORYRESET_H_

enum TSWStartupReason
    {
    ESWNone = 100,
    ESWRestoreFactorySet = 101,
    ESWLangSwitch = 102,
    ESWWarrantyTransfer = 103,
    ESWChargerConnected = 104,
    ESWRestoreFactorySetDeep = 105
    };

class SysStartup
    {
    public:
        IMPORT_C static TInt ShutdownAndRestart( const class TUid& aSource,
        									TSWStartupReason aReason );
    };

#endif /* DCFACTORYRESET_H_ */
