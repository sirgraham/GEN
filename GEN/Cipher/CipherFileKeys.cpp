/*------------------------------------------------------------------------------------------
//  CIPHERFILEKEYS.CPP
//
//  Cipher File Keys (Own keys cipher file for GEN)
//
//  Author            : Abraham J. Velez
//  Date Of Creation  : 19/03/2014 13:13:28
//  Last Modification :
//
//  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/


/*---- INCLUDES --------------------------------------------------------------------------*/

#include "XFactory.h"
#include "XMPInteger.h"
#include "XBuffer.h"
#include "XBER.h"
#include "XFileXML.h"

#include "Cipher.h"
#include "CipherRSA.h"

#include "CipherFileKeys.h"

#include "XMemory.h"

/*---- GENERAL VARIABLE ------------------------------------------------------------------*/


/*---- CLASS MEMBERS ---------------------------------------------------------------------*/


/*-------------------------------------------------------------------
//  CIPHERFILEKEYS::CIPHERFILEKEYS
*/
/**
//
//  Class Constructor CIPHERFILEKEYS
//
//  @author       Abraham J. Velez
//  @version      20/03/2014 9:27:46
//

//  @param        xpath :
*/
/*-----------------------------------------------------------------*/
CIPHERFILEKEYS::CIPHERFILEKEYS(XPATH& xpath)
{
  Clean();

  this->xpath  = xpath;

  xfilexml = new XFILEXML();
  if(!xfilexml) return;

  ReadAllFile();
}




/*-------------------------------------------------------------------
//  CIPHERFILEKEYS::~CIPHERFILEKEYS
*/
/**
//
//   Class Destructor CIPHERFILEKEYS
//
//  @author       Abraham J. Velez
//  @version      20/03/2014 9:28:10
//
*/
/*-----------------------------------------------------------------*/
CIPHERFILEKEYS::~CIPHERFILEKEYS()
{
  DeleteAllKeys();

  if(xfilexml) delete xfilexml;

  Clean();
}




/*-------------------------------------------------------------------
//  CIPHERFILEKEYS::HaveKey
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      20/03/2014 13:39:33
//
//  @return       bool :
//
//  @param        type :
*/
/*-----------------------------------------------------------------*/
bool CIPHERFILEKEYS::HaveKey(CIPHERKEYTYPE type)
{
  for(int c=0; c<(int)keys.GetSize(); c++)
    {
      CIPHERKEY* key = keys.Get(c);
      if(key)
        {
          if(key->GetType() == type)  return true;
        }
    }

  return false;
}



/*-------------------------------------------------------------------
//  CIPHERFILEKEYS::GetKey
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      20/03/2014 9:33:51
//
//  @return       CIPHERKEY* :
//
//  @param        type :
*/
/*-----------------------------------------------------------------*/
CIPHERKEY* CIPHERFILEKEYS::GetKey(CIPHERKEYTYPE type)
{
  for(int c=0; c<(int)keys.GetSize(); c++)
    {
      CIPHERKEY* key = keys.Get(c);
      if(key)
        {
          if(key->GetType() == type)  return key;
        }
    }

  return NULL;
}



/*-------------------------------------------------------------------
//  CIPHERFILEKEYS::AddKey
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      20/03/2014 9:30:08
//
//  @return       bool :
//
//  @param        key :
*/
/*-----------------------------------------------------------------*/
bool CIPHERFILEKEYS::AddKey(CIPHERKEY& key)
{
  if(HaveKey(key.GetType())) return false;

  switch(key.GetType())
    {
      case CIPHERKEYTYPE_UNKNOWN        : break;

      case CIPHERKEYTYPE_SYMMETRICAL  : { CIPHERKEYSYMMETRICAL* keysimetrical = new CIPHERKEYSYMMETRICAL();

                                          keysimetrical->CopyFrom((CIPHERKEYSYMMETRICAL*)&key);

                                          keys.Add(keysimetrical);
                                        }
                                        break;

      case CIPHERKEYTYPE_PUBLIC       : { CIPHERKEYRSAPUBLIC* keypublic = new CIPHERKEYRSAPUBLIC();

                                          keypublic->CopyFrom((CIPHERKEYRSAPUBLIC*)&key);
                                          keys.Add(keypublic);
                                        }
                                        break;

      case CIPHERKEYTYPE_PRIVATE      : { CIPHERKEYRSAPRIVATE* keyprivate = new CIPHERKEYRSAPRIVATE();

                                          keyprivate->CopyFrom((CIPHERKEYRSAPRIVATE*)&key);
                                          keys.Add(keyprivate);
                                        }
                                        break;
    }


  return true;
}



/*-------------------------------------------------------------------
//  CIPHERFILEKEYS::DeleteKey
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      20/03/2014 9:30:13
//
//  @return       bool :
//
//  @param        type :
*/
/*-----------------------------------------------------------------*/
bool CIPHERFILEKEYS::DeleteKey(CIPHERKEYTYPE type)
{
  for(int c=0; c<(int)keys.GetSize(); c++)
    {
      CIPHERKEY* key = keys.Get(c);
      if(key)
        {
          if(key->GetType() == type)
            {
              keys.Delete(key);
              delete key;
            }
        }
    }

  return true;
}



/*-------------------------------------------------------------------
//  CIPHERFILEKEYS::DeleteAllKeys
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      20/03/2014 9:30:20
//
//  @return       bool :
//
*/
/*-----------------------------------------------------------------*/
bool CIPHERFILEKEYS::DeleteAllKeys()
{
  if(keys.IsEmpty()) return false;

  keys.DeleteContents();

  keys.DeleteAll();

  return true;
}



/*-------------------------------------------------------------------
//  CIPHERFILEKEYS::UpdateFile
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      20/03/2014 9:30:25
//
//  @return       bool :
//
*/
/*-----------------------------------------------------------------*/
bool CIPHERFILEKEYS::UpdateFile()
{
  if(!xfilexml) return false;

  if(!xfilexml->Create(xpath))   return false;

  XFILEXMLELEMENT* noderoot = new XFILEXMLELEMENT(CIPHERFILEKEYS_NODENAME_ROOT);
  if(!noderoot) return false;

  XFILEXMLELEMENT* node   = NULL;
  XSTRING          string;
  int              index  = 0;

  node = noderoot->AddElement(CIPHERFILEKEYS_NODENAME_CREATOR);
  if(node)
    {
      XDATETIME* xdatetime = xfactory->CreateDateTime();
      if(xdatetime)
        {
          xdatetime->Read();

          node->AddAtribute(CIPHERFILEKEYS_ATTRNAME_CREATOR_CREATENAME , creatorname.Get());

          if(!creatordatetime.IsValidDate()) creatordatetime.CopyFrom(xdatetime);
          creatordatetime.GetDateTimeToString(XDATETIME_FORMAT_STANDARD, string);
          node->AddAtribute(CIPHERFILEKEYS_ATTRNAME_CREATOR_CREATEDATE , string.Get());

          creatorlastmodifieddatetime.CopyFrom(xdatetime);

          creatorlastmodifieddatetime.GetDateTimeToString(XDATETIME_FORMAT_STANDARD, string);
          node->AddAtribute(CIPHERFILEKEYS_ATTRNAME_CREATOR_LASTMODDATE , string.Get());

          xfactory->DeleteDateTime(xdatetime);
        }

      for(int c=0; c<(int)keys.GetSize(); c++)
        {
          CIPHERKEY* key = keys.Get(c);
          if(key)
            {
              if(key->GetType()!=CIPHERKEYTYPE_UNKNOWN)
                {
                  node = noderoot->AddElement(CIPHERFILEKEYS_NODENAME_CIPHERKEY);
                  if(node)
                    {
                      string.Format(__L("%d"), key->GetType());
                      node->AddAtribute(CIPHERFILEKEYS_ATTRNAME_CIPHERKEY_TYPEKEY, string.Get());

                      string.Format(__L("%d"), key->GetSizeInBits());
                      node->AddAtribute(CIPHERFILEKEYS_ATTRNAME_CIPHERKEY_SIZEBITS, string.Get());

                      switch(key->GetType())
                        {
                          case CIPHERKEYTYPE_UNKNOWN        : break;

                          case CIPHERKEYTYPE_SYMMETRICAL  : { CIPHERKEYSYMMETRICAL* keysimetrical = (CIPHERKEYSYMMETRICAL*)key;
                                                              XBUFFER* xbuffer = keysimetrical->Get();
                                                              if(xbuffer) string.ConvertHexStringFromBuffer((*xbuffer));

                                                              node->AddAtribute(CIPHERFILEKEYS_ATTRNAME_CIPHERKEY_KEY , string.Get());
                                                            }
                                                            break;

                          case CIPHERKEYTYPE_PUBLIC       : { CIPHERKEYRSAPUBLIC* keypublic = (CIPHERKEYRSAPUBLIC*)key;
                                                              XMPINTEGER          modulus;
                                                              XMPINTEGER          exponent;

                                                              keypublic->Get(modulus, exponent);

                                                              modulus.GetToString(16, string);
                                                              node->AddAtribute(CIPHERFILEKEYS_ATTRNAME_CIPHERKEY_MODULUS , string.Get());

                                                              exponent.GetToString(16, string);
                                                              node->AddAtribute(CIPHERFILEKEYS_ATTRNAME_CIPHERKEY_EXPONENT , string.Get());
                                                            }
                                                            break;

                          case CIPHERKEYTYPE_PRIVATE      : { CIPHERKEYRSAPRIVATE*  keyprivate = (CIPHERKEYRSAPRIVATE*)key;
                                                              XMPINTEGER            prime1factor;
                                                              XMPINTEGER            prime2factor;
                                                              XMPINTEGER            exponent;

                                                              keyprivate->Get(prime1factor, prime2factor, exponent);

                                                              prime1factor.GetToString(16, string);
                                                              node->AddAtribute(CIPHERFILEKEYS_ATTRNAME_CIPHERKEY_FACTOR1 , string.Get());

                                                              prime2factor.GetToString(16, string);
                                                              node->AddAtribute(CIPHERFILEKEYS_ATTRNAME_CIPHERKEY_FACTOR2 , string.Get());

                                                              exponent.GetToString(16, string);
                                                              node->AddAtribute(CIPHERFILEKEYS_ATTRNAME_CIPHERKEY_EXPONENT , string.Get());
                                                            }
                                                            break;
                        }
                    }
                }
            }
        }
    }

  bool status;

  xfilexml->SetRoot(noderoot);

  XVECTOR<XFILEXMLATTRIBUTE*>* CFGatributes = xfilexml->GetCFGAtributes();
  if(CFGatributes)
    {
      XFILEXMLATTRIBUTE* attribute;

      attribute = new XFILEXMLATTRIBUTE(__L("version"),__L("1.0"));
      if(attribute) CFGatributes->Add(attribute);

      //attribute = new XFILEXMLATTRIBUTE(__L("encoding"),__L("UTF-8"));
      //if(attribute) CFGatributes->Add(attribute);
    }

  status = xfilexml->EncodeAllLines();
  if(status) status = xfilexml->WriteAllFile();

  xfilexml->DeleteAllElements();

  xfilexml->Close();

  return status;
}




/*-------------------------------------------------------------------
//  CIPHERFILEKEYS::ExportToPEMFile
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      08/04/2014 12:13:06
//
//  @return       bool :
//
//  @param        key :
//  @param        publicPEM :
*/
/*-----------------------------------------------------------------*/
bool CIPHERFILEKEYS::ExportToPEMFile(CIPHERKEY* key, XSTRING& publicPEM)
{
  if(!key) return false;

  bool status = false;

  switch(key->GetType())
    {
      case CIPHERKEYTYPE_PUBLIC     : { XBER                  beroui;
                                        XBER                  bernull;
                                        XBER                  berbitstring;
                                        XBER                  bermodule;
                                        XBER                  berexponent;
                                        XBER                  berseq1;
                                        XBER                  berseq2;
                                        XBER                  berseq3;
                                        CIPHERKEYRSAPUBLIC*   publickey = (CIPHERKEYRSAPUBLIC*)key;
                                        XMPINTEGER            module;
                                        XMPINTEGER            exponent;
                                        XBUFFER               xbuffermodule;
                                        XBUFFER               xbuffermodule2;
                                        XBUFFER               xbufferexponent;
                                        XBUFFER               xbufferpem;
                                        XSTRING               string;
                                        XSTRING               stringbase64;
                                        XSTRING               _publicPEM;

                                        publickey->Get(module, exponent);

                                        module.GetToXBuffer(xbuffermodule2    , module.GetSize());
                                        exponent.GetToXBuffer(xbufferexponent , exponent.GetSize());


                                        xbuffermodule.Add((XBYTE)0);
                                        xbuffermodule.Add(xbuffermodule2);
                                        bermodule.SetINTEGER(xbuffermodule);
                                        berexponent.SetINTEGER(xbufferexponent);

                                        berseq2.Sequence_AddTo(bermodule);
                                        berseq2.Sequence_AddTo(berexponent);
                                        berseq2.SetType(berseq2.GetType()|(XBYTE)(XBERTYPE_ISCONSTRUCTED));

                                        xbufferpem.Add((XBYTE)0);
                                        berseq2.GetDump(xbufferpem);
                                        berbitstring.SetBITSTRING(xbufferpem);

                                        beroui.SetOID(__L("1.2.840.113549.1.1.1"));
                                        bernull.SetNULL();

                                        berseq1.Sequence_AddTo(beroui);
                                        berseq1.Sequence_AddTo(bernull);
                                        berseq1.SetType(berseq1.GetType()|(XBYTE)(XBERTYPE_ISCONSTRUCTED));

                                        berseq3.Sequence_AddTo(berseq1);
                                        berseq3.Sequence_AddTo(berbitstring);
                                        berseq3.SetType(berseq3.GetType()|(XBYTE)(XBERTYPE_ISCONSTRUCTED));

                                        xbufferpem.Delete();
                                        berseq3.GetDump(xbufferpem);

                                        string.Set(xbufferpem.Get(), xbufferpem.GetSize());
                                        string.ConvertToBase64(stringbase64);

                                        _publicPEM.Empty();
                                        _publicPEM += __L("-----BEGIN PUBLIC KEY-----\n");

                                        int index       = 0;
                                        int lincounter  = 0;

                                        while(index<(int)stringbase64.GetSize())
                                          {
                                            _publicPEM.Add(stringbase64.Get()[index]);
                                            index++;
                                            lincounter++;

                                            if(lincounter>=64)
                                              {
                                                _publicPEM.Add(__C('\n'));
                                                lincounter = 0;
                                              }
                                          }

                                        _publicPEM.Add(__C('\n'));
                                        _publicPEM += __L("-----END PUBLIC KEY-----\n");

                                        publicPEM = _publicPEM;
                                      }
                                      status = true;
                                      break;

      case CIPHERKEYTYPE_PRIVATE    : break;
    }

  return status;
}





/*-------------------------------------------------------------------
//  CIPHERFILEKEYS::ExportToPEMFile
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      08/04/2014 12:18:23
//
//  @return       bool :
//
//  @param        key :
//  @param        xpath :
*/
/*-----------------------------------------------------------------*/
bool CIPHERFILEKEYS::ExportToPEMFile(CIPHERKEY* key, XPATH& xpath)
{
  if(!key) return false;

  XSTRING publicPEM;

  if(!ExportToPEMFile(key, publicPEM)) return false;

  XFILE* xfile = xfactory->Create_File();
  if(xfile)
    {
      if(xfile->Create(xpath))
        {
          XSTRING_CREATEOEM(publicPEM, charstr)
          xfile->Write((XBYTE*)charstr, publicPEM.GetSize());
          XSTRING_DELETEOEM(charstr)
          xfile->Close();

          return true;
        }
    }

  return false;
}




/*-------------------------------------------------------------------
//  CIPHERFILEKEYS::ReadAllFile
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      20/03/2014 9:30:30
//
//  @return       bool :
//
*/
/*-----------------------------------------------------------------*/
bool CIPHERFILEKEYS::ReadAllFile()
{
  if(!xfilexml) return false;

  if(!xfilexml->Open(xpath))  return false;

  xfilexml->ReadAndDecodeAllLines();

  XFILEXMLELEMENT* noderoot = xfilexml->GetRoot();
  if(noderoot)
    {
      if(!noderoot->GetName().Compare(CIPHERFILEKEYS_NODENAME_ROOT))
        {
          XFILEXMLELEMENT* node   = NULL;
          int              index  = 0;

          do{ node = noderoot->GetElement(index);
              if(node)
                {
                  if(!node->GetName().Compare(CIPHERFILEKEYS_NODENAME_CREATOR))
                    {
                      XSTRING string;

                      node->GetValueAttribute(CIPHERFILEKEYS_ATTRNAME_CREATOR_CREATENAME  , creatorname);
                      if(node->GetValueAttribute(CIPHERFILEKEYS_ATTRNAME_CREATOR_CREATEDATE  , string))  creatordatetime.GetDateTimeFromString(string, XDATETIME_FORMAT_STANDARD);
                      if(node->GetValueAttribute(CIPHERFILEKEYS_ATTRNAME_CREATOR_LASTMODDATE , string))  creatorlastmodifieddatetime.GetDateTimeFromString(string, XDATETIME_FORMAT_STANDARD);
                    }
                   else
                    {
                      if(!node->GetName().Compare(CIPHERFILEKEYS_NODENAME_CIPHERKEY))
                        {
                          XSTRING           string;
                          int               index2 = 0;

                          if(node->GetValueAttribute(CIPHERFILEKEYS_ATTRNAME_CIPHERKEY_TYPEKEY  , string))
                            {
                              int typekey = string.ConvertToInt();

                              switch(typekey)
                                {
                                  case CIPHERKEYTYPE_UNKNOWN        : break;

                                  case CIPHERKEYTYPE_SYMMETRICAL  : { CIPHERKEYSYMMETRICAL* keysimetrical = new CIPHERKEYSYMMETRICAL();
                                                                      if(keysimetrical)
                                                                        {
                                                                          if(node->GetValueAttribute(CIPHERFILEKEYS_ATTRNAME_CIPHERKEY_KEY  , string))
                                                                            {
                                                                              XBUFFER xbuffer;

                                                                              string.ConvertHexStringToBuffer(xbuffer);
                                                                              keysimetrical->Set(xbuffer);

                                                                              keys.Add(keysimetrical);
                                                                            }
                                                                        }
                                                                    }
                                                                    break;

                                  case CIPHERKEYTYPE_PUBLIC       : { CIPHERKEYRSAPUBLIC* keypublic = new CIPHERKEYRSAPUBLIC();
                                                                      if(keypublic)
                                                                        {
                                                                          XMPINTEGER modulus;
                                                                          XMPINTEGER exponent;

                                                                          modulus.Ini();
                                                                          exponent.Ini();

                                                                          if(node->GetValueAttribute(CIPHERFILEKEYS_ATTRNAME_CIPHERKEY_MODULUS, string))
                                                                            {
                                                                              modulus.SetFromString(16, string);
                                                                              if(node->GetValueAttribute(CIPHERFILEKEYS_ATTRNAME_CIPHERKEY_EXPONENT, string))
                                                                                {
                                                                                  exponent.SetFromString(16, string);

                                                                                  keypublic->Set(modulus,exponent);

                                                                                  keys.Add(keypublic);
                                                                                }
                                                                            }

                                                                          modulus.End();
                                                                          exponent.End();
                                                                       }
                                                                    }
                                                                    break;

                                  case CIPHERKEYTYPE_PRIVATE      : { CIPHERKEYRSAPRIVATE* keyprivate = new CIPHERKEYRSAPRIVATE();
                                                                      if(keyprivate)
                                                                        {
                                                                          XMPINTEGER prime1factor;
                                                                          XMPINTEGER prime2factor;
                                                                          XMPINTEGER exponent;

                                                                          prime1factor.Ini();
                                                                          prime2factor.Ini();
                                                                          exponent.Ini();

                                                                          if(node->GetValueAttribute(CIPHERFILEKEYS_ATTRNAME_CIPHERKEY_FACTOR1, string))
                                                                            {
                                                                              prime1factor.SetFromString(16, string);

                                                                              if(node->GetValueAttribute(CIPHERFILEKEYS_ATTRNAME_CIPHERKEY_FACTOR2, string))
                                                                                {
                                                                                  prime2factor.SetFromString(16, string);

                                                                                  if(node->GetValueAttribute(CIPHERFILEKEYS_ATTRNAME_CIPHERKEY_EXPONENT, string))
                                                                                    {
                                                                                      exponent.SetFromString(16, string);

                                                                                      keyprivate->Set(prime1factor, prime2factor, exponent);

                                                                                      keys.Add(keyprivate);
                                                                                    }
                                                                                }
                                                                            }

                                                                          prime1factor.End();
                                                                          prime2factor.End();
                                                                          exponent.End();
                                                                        }
                                                                      }
                                                                      break;

                                }
                            }
                        }
                    }

                  index++;
                }

            } while(node);
        }
    }

  xfilexml->Close();

  return true;
}
