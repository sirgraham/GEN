
/*------------------------------------------------------------------------------------------
//  GRPPARTICLESYSTEM.CPP
//
//  Holds cluster data for sprite clouds
//
//  Author            : Diego Martinez Ruiz de Gaona
//  Date Of Creation  : 15/10/2015 13:50:42
//  Last Modification :
//
//  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/


/*---- INCLUDES --------------------------------------------------------------------------*/

#include "GRPParticleSystem.h"
#include "GRPScene.h"

#include "XMemory.h"

/*---- GENERAL VARIABLE ------------------------------------------------------------------*/


/*---- CLASS MEMBERS ---------------------------------------------------------------------*/

/*-------------------------------------------------------------------
//  GRPPARTICLESYSTEM::Draw
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      15/10/2015 13:54:24
//
//  @return       bool :
//
//  @param        view :
//  @param        viewprojection :
//  @param        projection :
//  @param        shaderlink :
*/
/*-----------------------------------------------------------------*/
bool  GRPPARTICLESYSTEM::Draw(  GRPMATRIX&  view, GRPMATRIX*  viewprojection, GRPMATRIX* projection)
{
  int n=this->Pools.GetSize();
  for (int e=n-1;e>=0;e--)
  {
    this->Pools.FastGet(e)->Draw(view,viewprojection,projection);
  }
  return true;
}

/*-------------------------------------------------------------------
//  GRPPARTICLESYSTEM::Step
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      15/10/2015 13:54:31
//
//  @return       bool :
//
*/
/*-----------------------------------------------------------------*/
bool  GRPPARTICLESYSTEM::Step()
{
  int n=this->Pools.GetSize();
  for (int e=0;e<n;e++)
  {
    this->Pools.FastGet(e)->Step();
  }
  return true;
}

/*-------------------------------------------------------------------
//  GRPPARTICLESYSTEM::GetPools
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      15/10/2015 13:54:37
//
//  @return       XVECTOR<GRPPARTICLEPOOL*>* :
//
*/
/*-----------------------------------------------------------------*/
XVECTOR<GRPPARTICLEPOOL*>*  GRPPARTICLESYSTEM::GetPools()
{
  return &this->Pools;
}

/*-------------------------------------------------------------------
//  GRPPARTICLESYSTEM::GetPool
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      15/10/2015 13:54:42
//
//  @return       GRPPARTICLEPOOL* :
//
*/
/*-----------------------------------------------------------------*/
GRPPARTICLEPOOL*  GRPPARTICLESYSTEM::GetPool(int x)
{
    return this->Pools.FastGet(x);
}

/*-------------------------------------------------------------------
//  GRPPARTICLESYSTEM::CreatePool
*/
/**
//
//  Creates a new pool
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      15/10/2015 14:06:16
//
//  @return       GRPPARTICLEPOOL* :
//
*/
/*-----------------------------------------------------------------*/
GRPPARTICLEPOOL*  GRPPARTICLESYSTEM::CreatePool()
{
  GRPPARTICLEPOOL* pool=new GRPPARTICLEPOOL();
  if (pool!=NULL)
    if (!this->Pools.Add(pool))
    {
      delete(pool);
      return NULL;
    }
  pool->SetNode(scene->CreateNode());
  return pool;
}