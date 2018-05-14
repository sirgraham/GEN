//------------------------------------------------------------------------------------------
//  DIOSTREAMUDP.H
//
/**
// \class
//
//  Data IO Stream UDP class
//
//  @author  Abraham J. Velez
//  @version 23/05/2002
*/
//  GEN  Copyright (C).  All right reserved.
//------------------------------------------------------------------------------------------

#ifndef _DIOSTREAMUDP_H_
#define _DIOSTREAMUDP_H_


//---- INCLUDES ----------------------------------------------------------------------------

#include "XVector.h"
#include "XBuffer.h"

#include "DIOStream.h"

//---- DEFINES & ENUMS  --------------------------------------------------------------------

#define DIOSTREAMUDP_NOTFOUND             -1

//---- CLASS -------------------------------------------------------------------------------

class XFACTORY;
class XTIMER;
class XMUTEX;
class DIOFACTORY;
class DIOURL;
class DIOSTREAMUDPCONFIG;


class DIOSTREAMUDPDATAGRAM
{
  public:
                                      DIOSTREAMUDPDATAGRAM                      ()
                                      {
                                        Clean();
                                        data = new XBUFFER();
                                      }

    virtual                          ~DIOSTREAMUDPDATAGRAM                      ()
                                      {
                                        if(data) delete data;

                                        Clean();
                                      }


    bool                              IsToSend                                  ()                                { return istosend;                      }
    void                              SetIsToSend                               (bool istosend)                   { this->istosend = istosend;            }

    XSTRING*                          GetAddress                                ()                                { return &address;                      }
    bool                              SetAddress                                (XCHAR* address)
                                      {
                                        if(!address) return false;
                                        this->address = address;
                                        return true;
                                      }
    bool                              SetAddress                                (XSTRING& address)                { return SetAddress(address.Get());     }

    XWORD                             GetPort                                   ()                                { return port;                          }
    void                              SetPorts                                  (XWORD port)                      { this->port = port;                    }

    XBUFFER*                          GetData                                   ()                                { return data;                          }
    bool                              SetData                                   (XBYTE* data,int size)
                                      {
                                        if(!this->data) return false;
                                        this->data->Delete();
                                        this->data->Add(data,size);

                                        return true;
                                      }

    void                              SetData                                   (XBUFFER& data)                   { SetData(data.Get(), data.GetSize());  }

  private:

    void                              Clean                                     ()
                                      {
                                        istosend = false;
                                        address.Empty();
                                        port     = 0;
                                        data     = NULL;
                                      }

    bool                              istosend;
    XSTRING                           address;
    XWORD                             port;
    XBUFFER*                          data;
};




class DIOSTREAMUDP : public DIOSTREAM
{
  public:
                                      DIOSTREAMUDP                              ();
    virtual                          ~DIOSTREAMUDP                              ();

    DIOSTREAMCONFIG*                  GetConfig                                 ();
    bool                              SetConfig                                 (DIOSTREAMCONFIG* config);

    virtual bool                      Open                                      ()                                = 0;

    bool                              ReadDatagram                              (XSTRING& address, XWORD& port, XBUFFER& xbuffer);

    bool                              WriteDatagram                             (XSTRING& address, XWORD port, XBYTE* buffer, int size);
    bool                              WriteDatagram                             (XSTRING& address, XWORD port, XBUFFER& xbuffer);

    virtual bool                      Disconnect                                ()                                = 0;
    virtual bool                      Close                                     ()                                = 0;

    bool                              ResetXBuffers                             ();
    bool                              ResetInXBuffer                            ();
    bool                              ResetOutXBuffer                           ();

    DIOURL*                           GetHost                                   ();

    bool                              WaitToWriteDatagramsEmpty                 (int timeout);
    int                               WaitToGetFirstDatagram                    (bool tosend, int timeout);

    XVECTOR<DIOSTREAMUDPDATAGRAM*>*   GetDatagramsVector                        ()                 { return &datagrams;                      };

    bool                              DeleteDatagram                            (int index);
    bool                              DeleteAllDatagrams                        ();

  protected:

    bool                              AddDatagram                               (bool istosend, XCHAR* address, XWORD port, XBYTE* data, int size);
    bool                              AddDatagram                               (bool istosend, XSTRING& address, XWORD port, XBYTE* data, int size);

    DIOSTREAMUDPDATAGRAM*             GetDatagram                               (int index);
    int                               GetFirstDatagram(bool issend);

    DIOSTREAMUDPCONFIG*               config;
    DIOURL*                           host;

  private:

    void                              Clean                                     ();

    XMUTEX*                           datagramsmutex;
    XVECTOR<DIOSTREAMUDPDATAGRAM*>    datagrams;
};


//---- INLINE FUNCTIONS --------------------------------------------------------------------

#endif
