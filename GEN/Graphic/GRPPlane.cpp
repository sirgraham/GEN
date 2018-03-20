/*------------------------------------------------------------------------------------------
// GRPPLANE.CPP
*/
/**
// \class
//
// PLane functions
//
//	@author: No se ha podido abrir el fichero de author
//
//	Date of Creation : 18/12/2017 14:01:20 
//	Last Modification : 
*/
/* (C) Copyright . All right reserved.
/*------------------------------------------------------------------------------------------*/


/*---- INCLUDES --------------------------------------------------------------------------*/

#include "GRPPlane.h"

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/

/*---- CLASS -----------------------------------------------------------------------------*/


void GRPPLANE::Set(GRPVECTOR v1, GRPVECTOR v2, GRPVECTOR v3)
{
	GRPVECTOR aux1, aux2;

	aux1 = v1 - v2;
	aux2 = v3 - v2;

	normal.CrossProduct(&aux2, &aux1);
	normal.Normalize();

	d = (GRPVECTOR::DotProduct(normal, v2));
}

float GRPPLANE::distance(GRPVECTOR v)
{
	return float(-float(d) + GRPVECTOR::DotProduct(normal, v));
}

void GRPPLANE::Normalize()
{
	double mag;
	mag = sqrt(a*a + b*b + c*c);
	a = a / mag;
	b = b / mag;
	c = c / mag;
	d = d / mag;
}
