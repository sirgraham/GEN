//------------------------------------------------------------------------------------------
//	IFAFEATURES.H
//	
/**	
// \class 
//   
//  Features code for IFA functions
//   
//	@author	 Abraham J. Velez
//	@version 30/03/2004 16:09:45
*/	
//	GEN  Copyright (C).  All right reserved.			 
//------------------------------------------------------------------------------------------
	
#ifndef _IFAFEATURES_H_
#define _IFAFEATURES_H_
	
	
//---- INCLUDES ----------------------------------------------------------------------------
	
	
//---- DEFINES & ENUMS  --------------------------------------------------------------------

// Generic
#define IFAFEATURE_NONE								0x00000000
#define IFAFEATURE_BACKGROUND					0x00000001
#define IFAFEATURE_TRANSPARENT				0x00000002

// Scroll
#define IFAFEATURE_HLEFTINDICATOR			0x00000004
#define IFAFEATURE_HRIGHTINDICATOR		0x00000008
#define IFAFEATURE_HBARINDICATOR			0x00000010
#define IFAFEATURE_VTOPINDICATOR			0x00000020
#define IFAFEATURE_VBOTTONINDICATOR		0x00000040
#define IFAFEATURE_VBARINDICATOR			0x00000080
#define IFAFEATURE_INDICATORSIFNEED		0x00000100	
#define IFAFEATURE_HINDICATORS				IFAFEATURE_HLEFTINDICATOR | IFAFEATURE_HRIGHTINDICATOR  | IFAFEATURE_HBARINDICATOR 
#define IFAFEATURE_VINDICATORS				IFAFEATURE_VTOPINDICATOR	| IFAFEATURE_VBOTTONINDICATOR	| IFAFEATURE_VBARINDICATOR
#define IFAFEATURE_ALLINDICATORS		  IFAFEATURE_HINDICATORS    | IFAFEATURE_VINDICATORS

// Form
#define IFAFEATURE_CAPTION						0x00000200

// Menus
#define IFAFEATURE_CICLYC							0x00000400
#define IFAFEATURE_SELECTBAR					0x00000800
#define IFAFEATURE_SELECTBARSIZEOVER	0x00001000
#define IFAFEATURE_SELECTBARSIZEFULL	0x00002000
#define IFAFEATURE_CANCELACTIVE				0x00004000
#define IFAFEATURE_CANCELTOSELECT			0x00008000

// Progress Bar
#define IFAFEATURE_VERTICAL						0x00010000
#define IFAFEATURE_TEXTPERCENT				0x00020000
#define IFAFEATURE_PROGRESSCOLOR			0x00040000

	
//---- CLASS -------------------------------------------------------------------------------
	
	
//---- INLINE FUNCTIONS --------------------------------------------------------------------
	
#endif




