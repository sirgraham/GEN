//------------------------------------------------------------------------------------------
//  XFACTORYMAC.H
//
/**
// \class
//
//  Defines for Platforms Factorys
//
//  @author  Abraham J. Velez
//  @version 13/08/2002
*/
//  GEN  Copyright (C).  All right reserved.
//------------------------------------------------------------------------------------------

#ifndef _XFACTORYMAC_H_
#define _XFACTORYMAC_H_


//---- INCLUDES ----------------------------------------------------------------------------

//---- DEFINES & ENUMS  --------------------------------------------------------------------

#define CREATEFUNC(CLASSNAME,GENERIC,SPECIFIC,NAME)   GENERIC* CLASSNAME::NAME()\
                                                      {\
                                                        SPECIFIC* _class = new SPECIFIC();\
                                                        return (GENERIC*)_class;\
                                                      }

#define DELETEFUNC(CLASSNAME,GENERIC,SPECIFIC,NAME)   bool CLASSNAME::NAME(GENERIC* _class)\
                                                      {\
                                                        if(!_class) return false;\
                                                        SPECIFIC* _class2 = (SPECIFIC*)_class;\
                                                        delete _class2;\
                                                        return true;\
                                                      }

//---- CLASS -------------------------------------------------------------------------------


//---- INLINE FUNCTIONS --------------------------------------------------------------------

#endif



