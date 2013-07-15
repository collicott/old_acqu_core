#ifndef __ROOTEPICSTYPES__
#define __ROOTEPICSTYPES__

//to define ezca types for root independently of any EPICS installation

enum EepicsType{                    EepicsBYTE, EepicsSTRING, EepicsSHORT, EepicsLONG, EepicsFLOAT, EepicsDOUBLE, EepicsNULL};
const int   epicsTypeSize[]   = {          1,           32,           2,          8,           4,            8,          0};
const char *epicsTypeName[]   = {"epicsBYTE","epicsSTRING","epicsSHORT","epicsLONG","epicsFLOAT","epicsDOUBLE",       NULL};


#endif
