/*------------------------------------------------------------------------------------------
//  GRPBUFFER.H
*/
/**
// \class
//
//  Graphics Buffer Class
//
//  @author  Diego Martinez Ruiz de Gaona
//  @version 30/07/2013 18:43:59
*/
/*  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/

#ifndef _GRPBUFFER_H_
#define _GRPBUFFER_H_


/*---- INCLUDES --------------------------------------------------------------------------*/

#include <math.h>
//#include <cmath>

#include "XBase.h"

#include "XMemory.h"

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/

enum
{
  COVERTYPE_SHIFT = 8                     ,
  COVERTYPE_SIZE  = 1 << COVERTYPE_SHIFT  ,
  COVERTYPE_MASK  = COVERTYPE_SIZE - 1    ,
  COVERTYPE_NONE  = 0                     ,
  COVERTYPE_FULL  = COVERTYPE_MASK
};


typedef XBYTE COVERTYPE;


/*---- CLASS -----------------------------------------------------------------------------*/




template<class T>
struct ROWINFO
{
  public:
                              ROWINFO                 ()
                              {
                                Clean();
                              }


                              ROWINFO                 (int x1, int x2, T* ptr)
                              {
                                Clean();

                                this->x1  = x1;
                                this->x2  = x2;
                                this->ptr = ptr;
                              }


    virtual                  ~ROWINFO                 ()
                              {
                                Clean();
                              }


    void                      Clean                   ()
                              {
                                x1  = 0;
                                x2  = 0;
                                ptr = NULL;
                              }


    int                       x1;
    int                       x2;
    T*                        ptr;
};



template<class T>
struct CONST_ROWINFO
{
  public:
                              CONST_ROWINFO           ()
                              {
                                Clean();
                              }


                              CONST_ROWINFO           (int x1, int x2, T* ptr)
                              {
                                Clean();

                                this->x1  = x1;
                                this->x2  = x2;
                                this->ptr = ptr;
                              }


    virtual                  ~CONST_ROWINFO           ()
                              {
                                Clean();
                              }


    void                      Clean                   ()
                              {
                                x1  = 0;
                                x2  = 0;
                                ptr = NULL;
                              }


    int                       x1;
    int                       x2;
    const T*                  ptr;
};



template<class T>
class GRPBUFFERMINMEM
{
  public:

    typedef CONST_ROWINFO<T>  ROWDATA;


                              GRPBUFFERMINMEM         ()                            {  Clean();       }


                              GRPBUFFERMINMEM         (T* buffer, XDWORD width, XDWORD height, int stride)
                              {
                                Clean();
                                Set(buffer,width,height,stride);
                              }


    virtual                  ~GRPBUFFERMINMEM         ()
                              {
                                Clean();
                              }


    const T*                  GetBuffer                 ()                            { return buffer;  }
    XDWORD                    GetWidth                  ()                            { return width;   }
    XDWORD                    GetHeight                 ()                            { return height;  }


    int                       GetStride                 ()                            { return stride;  }
    XDWORD                    GetStrideAbsolute         () const
                              {
                                  return (stride < 0) ? XDWORD(-stride) : XDWORD(stride);
                              }


    bool                      Set                       (T* buffer, XDWORD width, XDWORD height, int stride)
                              {
                                if(!buffer) return false;

                                this->buffer  = buffer;
                                this->start   = buffer;
                                this->width   = width;
                                this->height  = height;
                                this->stride  = stride;

                                if(stride < 0)
                                  {
                                    start = buffer - int(height - 1) * stride;
                                  }

                                return true;
                              }

    T*                        Row                       (XDWORD, XDWORD y, XDWORD)    { return start + (y * stride);          }
    T*                        Row                       (XDWORD y)                    { return start + (y * stride);          }
    const T*                  Row                       (XDWORD y) const              { return start + (y * stride);          }
    ROWDATA                   RowData                   (XDWORD y) const              { return ROWDATA(0, width-1, Row(y));   }

    void                      Clear                     (T value)
                              {
                                XDWORD y;
                                XDWORD w      = width();
                                XDWORD stride = GetStrideAbsolute();

                                for(y = 0; y < height(); y++)
                                  {
                                    T* p = Row(0, y, w);
                                    XDWORD x;

                                    for(x = 0; x < stride; x++)
                                      {
                                        *p++ = value;
                                      }
                                  }
                              }


  private:

    void                      Clean                     ()
                              {
                                buffer    = NULL;
                                start     = 0;
                                width     = 0;
                                height    = 0;
                                stride    = 0;
                                maxheight = 0;
                              }


    T*                        buffer;                   // Pointer to renrdering buffer
    T*                        start;                    // Pointer to first pixel depending on stride
    XDWORD                    width;                    // Width in pixels
    XDWORD                    height;                   // Height in pixels
    int                       stride;                   // Number of bytes per row. Can be < 0
    XDWORD                    maxheight;                // The maximal height (currently allocated)
};




template<class T>
class GRPBUFFERFAST
{
  public:

    typedef CONST_ROWINFO<T>  ROWDATA;

                              GRPBUFFERFAST           ()                      {  Clean();       }


                              GRPBUFFERFAST           (T* buffer, XDWORD width, XDWORD height, int stride)
                              {
                                Clean();
                                Set(buffer,width,height,stride);
                              }


    virtual                  ~GRPBUFFERFAST           ()
                              {
                                delete [] rows;
                                Clean();
                              }


    const T*                  GetBuffer                 ()                      { return buffer;  }
    XDWORD                    GetWidth                  ()                      { return width;   }
    XDWORD                    GetHeight                 ()                      { return height;  }


    int                       GetStride                 ()                      { return stride;  }
    XDWORD                    GetStrideAbsolute         () const
                              {
                                  return (stride < 0) ? XDWORD(-stride) : XDWORD(stride);
                              }


    bool                      Set                       (T* buffer, XDWORD width, XDWORD height, int stride)
                              {
                                if(!buffer) return false;

                                this->buffer  = buffer;
                                this->width   = width;
                                this->height  = height;
                                this->stride  = stride;

                                if(height > maxheight)
                                  {
                                    delete [] rows;
                                    rows = new T* [maxheight = height];
                                  }

                                T* row_ptr = buffer;

                                if(stride < 0)
                                  {
                                    row_ptr = buffer - int(height - 1) * stride;
                                  }

                                T** _rows = rows;

                                while(height--)
                                  {
                                    *_rows++ = row_ptr;
                                     row_ptr += stride;
                                  }

                                return true;
                              }


    T*                        Row                       (int, int y, unsigned)  { return rows[y];                           }
    T*                        Row                       (XDWORD y)              { return rows[y];                           }
    const T*                  Row                       (XDWORD y) const        { return rows[y];                           }
    ROWDATA                   RowData                   (int y) const           { return ROWDATA(0, width-1, rows[y]);      }

    T const* const*           Rows                      ()                      { return rows;                              }

    void                      Clear                     (T value)
                              {
                                XDWORD y;
                                XDWORD w      = width();
                                XDWORD stride = GetStrideAbsolute();

                                for(y = 0; y < height(); y++)
                                  {
                                    T* p = Row(0, y, w);
                                    XDWORD x;

                                    for(x = 0; x < stride; x++)
                                      {
                                        *p++ = value;
                                      }
                                  }
                              }


  private:

    void                      Clean                     ()
                              {
                                buffer    = NULL;
                                rows      = NULL;
                                width     = 0;
                                height    = 0;
                                stride    = 0;
                                maxheight = 0;
                              }


    T*                        buffer;                   // Pointer to renrdering buffer
    T**                       rows;                     // Pointers to each row of the buffer
    XDWORD                    width;                    // Width in pixels
    XDWORD                    height;                   // Height in pixels
    int                       stride;                   // Number of bytes per row. Can be < 0
    XDWORD                    maxheight;                // The maximal height (currently allocated)
};



typedef GRPBUFFERMINMEM<XBYTE>    GRPBUFFER;

//typedef GRPBUFFERFAST<XBYTE>      GRPBUFFER;

/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/


inline int                    iRound                    (double v)      { return int((v < 0.0) ? v - 0.5 : v + 0.5);  }
inline int                    uRound                    (double v)      { return unsigned(v + 0.5);                   }
inline XDWORD                 uFloor                    (double v)      { return XDWORD(v);                           }
inline XDWORD                 uCeil                     (double v)      { return unsigned(ceil(v));                   }


#endif

