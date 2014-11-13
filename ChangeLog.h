/*
2.85
- Cleaned Code
- MeterFailure implemented

2.84
- worked on detection of meterfailure

2.83
- Github #56 msoverflow fixed
- ErrorReport=relayFailure

2.82
- implementd new library SerialCommandFCS2
- Update serial_functions: fl2str
- Baudrate changed to 9600

2.81
- Fixed issue #20 checked

2.80
- if lock is not able to close, try again in 2,5 seconds and blink cable led yellow. -> issue #27

2.79
- setVentilationPermission #38 fixed 

2.78
- implemented (VehicleStatus == 'D' && VentilationPermission ==true) in chargecontroll

2.77
- Updated readProx() -> issue 48

2.76
- Issue 55; Changed all the "char buffer1[len1+1]" to "char buffer1[11]"

2.75
- ErrorReport=powerFail(cs) at powerdown
- Linestate in function RelayStatus #31
- worked on Stopcharge results in unlock of cable #36. Code gets to complicated if implement diffrent then specs. Updated but functionality remains the same. 


2.74
- implemented Heartbeat. UPDATE LIBRARY SerialCommandFCS
2.73
- Fixed issue 'Unwanted push message in initial state #54'
2.72
- Fixed issue 'Unwanted push message in initial state #54' (bug found)
- Added changelog.h to Arduino project
2.71
- Implemented Hans his changes on Checksum issue #39
2.70
- added the LedOveride
- update whole led code
2.6 and before
- fixed issue #51, #50
- Bug #17 solved cmdEnclosureStatus
*/
