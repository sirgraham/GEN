/**-------------------------------------------------------------------------------------------------------------------
*
* @file        GRPScreen.cpp
*
* @class       GRPSCREEN
* @brief       Graphics Screen Class
* @ingroup     GRAPHIC
*
* @author      Abraham J. Velez 
* @date        01/06/2018 12:58:23
*
* @copyright   Copyright(c) 2005 - 2018 GEN Group.
*
* @cond
* Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated
* documentation files(the "Software"), to deal in the Software without restriction, including without limitation
* the rights to use, copy, modify, merge, publish, distribute, sublicense, and/ or sell copies of the Software,
* and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in all copies or substantial portions of
* the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO
* THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
* @endcond
*
*---------------------------------------------------------------------------------------------------------------------*/

/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/

#ifdef GRPVIEWPORTS_ACTIVE
#include "GRPViewport.h"
#endif

#include "GRPScreen.h"

#include "XMemory.h"

/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/

/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         GRPSCREEN::GRPSCREEN()
* @brief      Constructor
* @ingroup    GRAPHIC
*
* @author     Abraham J. Velez 
* @date       01/06/2018 12:28:39
*
* @return     Does not return anything. 
*
*---------------------------------------------------------------------------------------------------------------------*/
GRPSCREEN::GRPSCREEN()
{
  Clean();

  RegisterEvent(GRPXEVENTTYPE_SCREEN_CHANGEPOSITION);
  RegisterEvent(GRPXEVENTTYPE_SCREEN_CHANGESIZE);
  RegisterEvent(GRPXEVENTTYPE_SCREEN_CHANGEFOCUS);
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         GRPSCREEN::~GRPSCREEN()
* @brief      Destructor
* @note       VIRTUAL
* @ingroup    GRAPHIC
*
* @author     Abraham J. Velez 
* @date       01/06/2018 12:28:57
*
* @return     Does not return anything. 
*
*---------------------------------------------------------------------------------------------------------------------*/
GRPSCREEN::~GRPSCREEN()
{
  DeRegisterEvent(GRPXEVENTTYPE_SCREEN_CHANGEFOCUS);
  DeRegisterEvent(GRPXEVENTTYPE_SCREEN_CHANGESIZE);
  DeRegisterEvent(GRPXEVENTTYPE_SCREEN_CHANGEPOSITION);

  Clean();
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         GRPSCREENTYPE GRPSCREEN::GetType()
* @brief      Get Screen type
* @ingroup    GRAPHIC
*
* @author     Abraham J. Velez 
* @date       01/06/2018 12:29:09
*
* @return     GRPSCREENTYPE : Screen type
*
*---------------------------------------------------------------------------------------------------------------------*/
GRPSCREENTYPE GRPSCREEN::GetType()
{
  return type;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool GRPSCREEN::IsValid()
* @brief      Is valid screen
* @ingroup    GRAPHIC
*
* @author     Abraham J. Velez 
* @date       01/06/2018 12:30:21
*
* @return     bool : true if is succesful. 
*
*---------------------------------------------------------------------------------------------------------------------*/
bool GRPSCREEN::IsValid()
{
  return isvalid;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool GRPSCREEN::IsActive()
* @brief      Is screen active
* @ingroup    GRAPHIC
*
* @author     Abraham J. Velez 
* @date       01/06/2018 12:30:43
*
* @return     bool : true if is succesful. 
*
*---------------------------------------------------------------------------------------------------------------------*/
bool GRPSCREEN::IsActive()
{
  return isactive;
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool GRPSCREEN::SetPropertys(int width, int height, int stride, GRPPROPERTYMODE mode)
* @brief      
* @ingroup    GRAPHIC
*
* @author     Abraham J. Velez 
* @date       01/06/2018 12:31:00
*
* @param[in]  width : 
* @param[in]  height : 
* @param[in]  stride : 
* @param[in]  mode : 
*
* @return     bool : true if is succesful. 
*
*---------------------------------------------------------------------------------------------------------------------*/
bool GRPSCREEN::SetPropertys(int width, int height, int stride, GRPPROPERTYMODE mode)
{
  SetWidth(width);
  SetHeight(height);
  SetStride(stride);
  SetMode(mode);

  return true;
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool GRPSCREEN::IsFullScreen()
* @brief      
* @ingroup    GRAPHIC
*
* @author     Abraham J. Velez 
* @date       01/06/2018 12:31:29
*
* @return     bool : true if is succesful. 
*
*---------------------------------------------------------------------------------------------------------------------*/
bool GRPSCREEN::IsFullScreen()
{
  return isfullscreen;
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void GRPSCREEN::SetIsFullScreen(bool isfullscreen)
* @brief      
* @ingroup    GRAPHIC
*
* @author     Abraham J. Velez 
* @date       01/06/2018 12:31:43
*
* @param[in]  isfullscreen : 
*
* @return     void : does not return anything. 
*
*---------------------------------------------------------------------------------------------------------------------*/
void GRPSCREEN::SetIsFullScreen(bool isfullscreen)
{
  this->isfullscreen = isfullscreen;
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool GRPSCREEN::Resize(int width, int height)
* @brief      
* @ingroup    GRAPHIC
*
* @author     Abraham J. Velez 
* @date       01/06/2018 12:31:53
*
* @param[in]  width : 
* @param[in]  height : 
*
* @return     bool : true if is succesful. 
*
*---------------------------------------------------------------------------------------------------------------------*/
bool GRPSCREEN::Resize(int width, int height)
{
  return true;
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void GRPSCREEN::ShowCursor(bool active)
* @brief      
* @ingroup    GRAPHIC
*
* @author     Abraham J. Velez 
* @date       01/06/2018 12:55:09
*
* @param[in]  active : 
*
* @return     void : does not return anything. 
*
*---------------------------------------------------------------------------------------------------------------------*/
void GRPSCREEN::ShowCursor(bool active) 
{
    
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool GRPSCREEN::UpdateSize(int width, int height)
* @brief      
* @ingroup    GRAPHIC
*
* @author     Abraham J. Velez 
* @date       01/06/2018 12:32:14
*
* @param[in]  width : 
* @param[in]  height : 
*
* @return     bool : true if is succesful. 
*
*---------------------------------------------------------------------------------------------------------------------*/
bool GRPSCREEN::UpdateSize(int width, int height)
{
  this->width = width;
  this->height = height;

  #ifdef GRPVIEWPORTS_ACTIVE
  UpdateViewports(width, height);
  #endif

  GRPXEVENT grpevent(this, GRPXEVENTTYPE_SCREEN_CHANGESIZE);
  grpevent.SetSize(width, height);

  PostEvent(&grpevent);

  return true;
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void* GRPSCREEN::GetHandle()
* @brief      
* @ingroup    GRAPHIC
*
* @author     Abraham J. Velez 
* @date       01/06/2018 12:32:24
*
* @return     void* : 
*
*---------------------------------------------------------------------------------------------------------------------*/
void* GRPSCREEN::GetHandle()
{
  return handle;
}




/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void GRPSCREEN::SetDPI(float dpi)
* @brief      
* @ingroup    GRAPHIC
*
* @author     Abraham J. Velez 
* @date       01/06/2018 12:32:52
*
* @param[in]  dpi : 
*
* @return     void : does not return anything. 
*
*---------------------------------------------------------------------------------------------------------------------*/
void GRPSCREEN::SetDPI(float dpi)
{
  this->dpi = dpi;
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         float GRPSCREEN::GetDPI()
* @brief      
* @ingroup    GRAPHIC
*
* @author     Abraham J. Velez 
* @date       01/06/2018 12:32:59
*
* @return     float : 
*
*---------------------------------------------------------------------------------------------------------------------*/
float GRPSCREEN::GetDPI()
{
    return this->dpi;
}


#ifdef GRPVIEWPORTS_ACTIVE

/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         GRPVIEWPORT* GRPSCREEN::GetViewport(int index)
* @brief      
* @ingroup    GRAPHIC
*
* @author     Abraham J. Velez 
* @date       01/06/2018 12:32:32
*
* @param[in]  index : 
*
* @return     GRPVIEWPORT* : 
*
*---------------------------------------------------------------------------------------------------------------------*/
GRPVIEWPORT* GRPSCREEN::GetViewport(int index)
{
  return (viewports.Get(index));
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XVECTOR<GRPVIEWPORT*>* GRPSCREEN::GetViewports()
* @brief      
* @ingroup    GRAPHIC
*
* @author     Abraham J. Velez 
* @date       01/06/2018 12:32:39
*
* @return     XVECTOR<GRPVIEWPORT*>* : 
*
*---------------------------------------------------------------------------------------------------------------------*/
XVECTOR<GRPVIEWPORT*>* GRPSCREEN::GetViewports()
{
  return &viewports;
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool GRPSCREEN::AddViewport(GRPVIEWPORT* viewport)
* @brief      
* @ingroup    GRAPHIC
*
* @author     Abraham J. Velez 
* @date       01/06/2018 12:35:53
*
* @param[in]  viewport : 
*
* @return     bool : true if is succesful. 
*
*---------------------------------------------------------------------------------------------------------------------*/
bool GRPSCREEN::AddViewport(GRPVIEWPORT* viewport)
{
  viewport->SetWindowSize((GLFLOAT)this->GetWidth(),(GLFLOAT)this->GetHeight());

  return this->viewports.Add(viewport);
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool GRPSCREEN::UseViewport(int n, GRPVIEWPORT& viewport)
* @brief      
* @ingroup    GRAPHIC
*
* @author     Abraham J. Velez 
* @date       01/06/2018 12:36:07
*
* @param[in]  n : 
* @param[in]  viewport : 
*
* @return     bool : true if is succesful. 
*
*---------------------------------------------------------------------------------------------------------------------*/
bool GRPSCREEN::UseViewport(int n, GRPVIEWPORT& viewport)
{
  if(!viewports.Get(n))  return false;

  viewports.Get(n)->Select();
  viewport = (*viewports.Get(n));

  return true;
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool GRPSCREEN::UpdateViewports(int width, int height)
* @brief      
* @ingroup    GRAPHIC
*
* @author     Abraham J. Velez 
* @date       01/06/2018 12:36:22
*
* @param[in]  width : 
* @param[in]  height : 
*
* @return     bool : true if is succesful. 
*
*---------------------------------------------------------------------------------------------------------------------*/
bool GRPSCREEN::UpdateViewports(int width, int height)
{
  if(viewports.IsEmpty()) return false;

  for(XDWORD c=0; c<viewports.GetSize(); c++)
    {
      viewports.Get(c)->SetWindowSize((GLFLOAT)width, (GLFLOAT)height);
      viewports.Get(c)->Select();
    }

  return true;
}

#endif

/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void GRPSCREEN::Clean()
* @brief      Clean the attributes of the class: Default initialice
* @note       INTERNAL
* @ingroup    GRAPHIC
*
* @author     Abraham J. Velez 
* @date       01/06/2018 12:41:27
*
* @return     void : does not return anything. 
*
*---------------------------------------------------------------------------------------------------------------------*/
void GRPSCREEN::Clean()
{
  type            = GRPSCREENTYPE_UNKNOW;

  isvalid         = false;
  isactive        = false;
  isfullscreen    = false;

  positionx       = 0;
  positiony       = 0;

  handle          = NULL;
  dpi             = 0.0f;
}