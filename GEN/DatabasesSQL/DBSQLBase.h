/*------------------------------------------------------------------------------------------
//  DBSQLBASE.H
*/
/**
// \file
//
//  Base types for database
//
//  @author  Diego Martinez Ruiz de Gaona
//
//  Date Of Creation  : 10/08/2015 12:22:50
//  Last Modification :
*/
/*  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/

#ifdef DBSQL_ACTIVE

#ifndef _DBSQLBASE_H_
#define _DBSQLBASE_H_

/*---- INCLUDES --------------------------------------------------------------------------*/

#include "XString.h"
#include "XVector.h"
#include "XMap.h"
#include "XDateTime.h"

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/

#define   DBSQLSTRING      XSTRING
#define   DBSQLVECTOR      XVECTOR
#define   DBSQLMAP         XMAP
#define   DBSQLDATETIME    XDATETIME
#define   DBSQLMYSQL_OK    0

/*---- CLASS -----------------------------------------------------------------------------*/

class DBSQLVARIANT;


#endif

#endif