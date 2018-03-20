
/*------------------------------------------------------------------------------------------
//	PHYWORLD.CPP
//	
//	Physics abstract world
//   
//	Author						: No se ha podido encontrar el fichero 'MacrosAuthor.txt' en el directorio del IDE. No se han rellenado los datos para las macros.
//	Date Of Creation	: 26/05/2016 19:42:16
//	Last Modification	:	
//	
//	(C) Copyright No se ha podido encontrar el fichero 'MacrosAuthor.txt' en el directorio del IDE. No se han rellenado los datos para las macros.. All right reserved.
//----------------------------------------------------------------------------------------*/
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/
	
#include "phyworld.h"
	
/*---- GENERAL VARIABLE ------------------------------------------------------------------*/
	
/*---- CLASS MEMBERS ---------------------------------------------------------------------*/
/*-------------------------------------------------------------------
//	PHYWORLD::PHYWORLD
*/	
/**	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			26/05/2016 19:36:40
//	
*/
/*-----------------------------------------------------------------*/
PHYWORLD::PHYWORLD		() 
{
		Clean(); 
}

/*-------------------------------------------------------------------
//	PHYWORLD::~PHYWORLD
*/	
/**	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			26/05/2016 19:36:40
//	
*/
/*-----------------------------------------------------------------*/
PHYWORLD::~PHYWORLD		() 
{ 
		Clean(); 
}

/*-------------------------------------------------------------------
//	PHYWORLD::~PHYWORLD
*/	
/**	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			26/05/2016 19:36:40
//	
*/
/*-----------------------------------------------------------------*/
bool PHYWORLD::Destroy()
{
		DestroyEntities	();
		DestroyWorld		();
		return true;
}

/*-------------------------------------------------------------------
//	PHYWORLD::DestroyEntities
*/	
/**	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			26/05/2016 19:36:40
//	
// return bool 
*/
/*-----------------------------------------------------------------*/
bool PHYWORLD::DestroyEntities()
{
		for (XDWORD e=0;e<this->Entities.GetSize();e++)
		{
				this->Entities.Get(e)->Destroy();
				delete(this->Entities.Get(e));
		}

		Entities.DeleteAll();

		return true;
}
/*-------------------------------------------------------------------
//	PHYWORLD::AddEntity
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			26/05/2016 19:36:40
//	
//	@return 			bool : 
//
//  @param				entity : phisical entity to include
*/
/*-----------------------------------------------------------------*/
bool	PHYWORLD::AddEntity(PHYENTITY* entity)
{
return this->Entities.Add(entity);
}

/*-------------------------------------------------------------------
//	PHYWORLD::GetGravity
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			26/05/2016 19:36:40
//	
//	@return 			GRPVECTOR* : 
//
*/
/*-----------------------------------------------------------------*/
GRPVECTOR* PHYWORLD::GetGravity()											
{ 
		return &this->Gravity; 
}