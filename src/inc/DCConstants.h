#ifndef DCCONSTANTS_H
#define DCCONSTANTS_H

const TInt KDriveIconSlot = 0;
const TInt KMaxBuffer = 200;
const TInt KUnit = 1024;
const TInt KTotalImage = 3;
const TInt KBufSize = 50;
const TInt KMaxByte = 79872; //79872; //80486; //6205;
const TInt KAdjValue = 204800;
const TInt KMaxFileByteSize = 1073741824;
const TUint KRequiredChargeForFullClean = 30;
const TUint KRequiredChargeForPartialClean	= 20;
const TInt KPercentage = 100;
const TInt KNoOfOperationType = 2;
const TInt KMaximumFile = 250;
const TInt KDecimalPlaces = 2;
const TInt KDeleteOperation = 1;
const TInt KItemNumber = 2;
const TInt64 KThreshold = 0x1000;
const TInt KMMCRemoveIndicator = -6000;
const TUint KChargeForWarning = 15;
const TUint KChargeForAbort = 5;
const TInt KPhoneMaxSize = 100;
const TInt KTotalSelectContact = 10;
const TInt KPhoneCallEventIndicator = -15000;

_LIT( KMsgForPhoneMemory, "All data of this drive will be lost. "
							"Device will be rebooted. Continue?" );
_LIT( KMsgForExtMemory, "All data of this drive will be lost. Continue?" );
_LIT( KMsgForPartClean, "Free space will be cleaned. Continue?" );
_LIT( KMsgPartCleanComplete, "Partial clean complete" );
_LIT( KMsgFullCleanComplete, "Full clean complete" );
_LIT( KMsgForMaxBatteryLevel, "Battery must be full for this operation. ");
_LIT( KMsgForMinBatteryLevel, "Battery level is not sufficient to perform the operation. ");
_LIT( KMsgForSmsReceived, "SMS Received" );
_LIT( KMsgForAbortOperation, "Do you want to abort the operation?");
_LIT( KFullCleanText, "Fully cleaning..." );
_LIT( KPartialCleanText, "Partially cleaning..." );
_LIT( KNewLine, "\r\n" );
_LIT( KPercentageText, "%");
_LIT(KListItemFormat, "%S");
_LIT(KFirstItem, "Full clean");
_LIT(KNextItem, "Partial clean");
_LIT( KDriveC, "C:\\");
_LIT( KDriveE, "E:\\");
_LIT( KWriteDir, "BSDCTmp\\" );
_LIT ( KListFormatString, "%d\t" );
_LIT ( KSvgFileName, "\\resource\\apps\\S60DriveCleanser_svg.mif" );
_LIT ( KTab, "\t" );
_LIT ( KTextByte, " Byte" );
_LIT ( KTextKB, " KB" );
_LIT ( KTextMB, " MB" );
_LIT ( KTextGB, " GB" );
_LIT ( KSlash, "/" );
_LIT ( KLabelFree, "Free:" );
_LIT ( KLabelUsed, "Used:" );
_LIT ( KLabelTotal, "Total:" );
_LIT ( KLabelSeparator, "," );
_LIT ( KLabelPhoneMemory, "Phone Memory" );
_LIT ( KLabelExternalMemory, "External Memory" );
_LIT ( KLabelOf, " of ");
_LIT ( KLabelIsFree, " is free");
_LIT( KMSgForBatteryWarnMsg, "Battery low. Continuue?" );
_LIT( KMsgAbortForLowBattery,"Operation is aborted due to low battery");
_LIT( KMessageText,"S60DriveCleanser is a cool S60 application and might be "
		"useful to you.For further information, "
		"please visit www.bjitgroup.com");
_LIT( KInfoMsg, "During the operation Low/Full memory message may be shown."
													"Please ignore it" );



#endif //DCCONSTANTS_H

