/*------------------------------------------------------------------------------------------
// GRPStyleProcessorCSS.HPP
*/
/**
// \class
//
// Processes CSS
//
//  @author: Diego Martinez Ruiz de Gaona
//
//  Date of Creation : 22/02/2018 14:06:46
//  Last Modification :
*/
/* GEN  Copyright (C).  All right reserved.
/*------------------------------------------------------------------------------------------*/

#ifndef _GRPSTYLEPROCESSORCSS_HPP
#define _GRPSTYLEPROCESSORCSS_HPP

/*---- INCLUDES --------------------------------------------------------------------------*/

#include "XString.h"
#include "GRPColor.h"
#include "GRPWidgetStyle.h"

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/

/*---- CLASS -----------------------------------------------------------------------------*/
class GRPSTYLESHEET;
class GRPWIDGETSTYLECLASS;

class GRPSTYLEPROCESSORCSS
{
public:
    GRPSTYLEPROCESSORCSS();
    ~GRPSTYLEPROCESSORCSS();


    bool                      ParseStyle    (GRPSTYLESHEET*,XCHAR*);
    bool                      CleanStyle    (XSTRING*);
    GRPWIDGETSTYLECLASS*      CreateStyle   (XSTRING* name, XSTRING *data);
    bool                      Process       (GRPWIDGETSTYLECLASS* styleclass, XCHAR* stext);

    bool                      Init          (GRPSTYLESHEET*);
    bool                      InitDictionary();

    XMAP<GRPWIDGETSTYLETYPE, GRPWIDGETSTYLE*>* GetProperties() {  return &cssproperties;  }

    static GRPCOLORPALETTE*   GetPalette();

protected:

    static GRPCOLORPALETTE*                     csscolorpalette;
    XDICTIONARY<GRPWIDGETSTYLETYPE>             csstypes;
    XMAP<GRPWIDGETSTYLETYPE, GRPWIDGETSTYLE*>   cssproperties;



private:

};
/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/

#endif