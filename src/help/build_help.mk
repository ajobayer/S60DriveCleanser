# ============================================================================
#  Name	 : build_help.mk
#  Part of  : S60DriveCleanser
# ============================================================================
#  Name	 : build_help.mk
#  Part of  : S60DriveCleanser
#
#  Description: This make file will build the application help file (.hlp)
# 
# ============================================================================

do_nothing :
	@rem do_nothing


MAKMAKE :S60DriveCleanser_0x20027916.hlp

S60DriveCleanser_0x20027916.hlp : S60DriveCleanser.xml S60DriveCleanser.cshlp Custom.xml
	
		cshlpcmp S60DriveCleanser.cshlp
ifeq (WINS,$(findstring WINS, $(PLATFORM)))
	copy S60DriveCleanser_0x20027916.hlp $(EPOCROOT)epoc32\$(PLATFORM)\c\resource\help
endif

BLD : do_nothing

CLEAN :
	del S60DriveCleanser_0x20027916.hlp
	del S60DriveCleanser_0x20027916.hlp.hrh

LIB : do_nothing

CLEANLIB : do_nothing

RESOURCE : do_nothing
		
FREEZE : do_nothing

SAVESPACE : do_nothing

RELEASABLES :
	@echo S60DriveCleanser_0x20027916.hlp

FINAL : do_nothing
