/*------------------------------------------------------------------------------------------
//  GRPFILEGGF.H
*/
/**
// \file
//
//  loads a binary ggf file
//
//  @author  Imanol Celaya Ruiz de Alegria
//
//  Date Of Creation  : 12/11/2015 17:31:43
//  Last Modification :
*/
/*  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/

#ifndef _GRPFILEGGF_H_
#define _GRPFILEGGF_H_


/*---- INCLUDES --------------------------------------------------------------------------*/

#include "GRPGGFValue.h"

#include "GRPFile3D.h"
#include "XMap.h"
#include "XVector.h"
#include "XBuffer.h"
#include "XBinaryMap.h"

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/

/*---- CLASS -----------------------------------------------------------------------------*/

struct GGFHEADER
{
  XDWORD                                magic;
  XDWORD                                versionmayor;
  XDWORD                                versionminor;

  XDWORD                                Size                                  ()                                      { return 3*sizeof(XDWORD); }
};

struct GGFINFO
{
  XDWORD                                ptrsize;
  XDWORD                                datasize;
  XDWORD                                hash;

  XDWORD                                Size                                  ()                                      { return sizeof(XDWORD) + sizeof(XDWORD) + sizeof(XDWORD); }
};

// Seek Map, simple for now
class GGFSEEKMAP
{
  public:
                                        GGFSEEKMAP                            (GGFINFO& info) : info(info)
                                        {
                                          Clean();
                                        }

                                       ~GGFSEEKMAP                            ()
                                        {
                                          Clean();
                                        }

    bool                                Add                                   (XQWORD ptr, XQWORD offset)
                                        {
                                          seekmaptree.Add(ptr, offset);
                                          return true;
                                        }

    XQWORD                              Get                                   (XQWORD ptr)
                                        {
                                          return seekmaptree.Get(ptr);
                                        }


    XDWORD                              GetSize                               ()
                                        {
                                          return seekmaptree.GetSize();
                                        }


    XDWORD                              Size                                  ()
                                        {
                                          return seekmaptree.GetSize()*(sizeof(XQWORD) + info.ptrsize) + sizeof(XDWORD);
                                        }


    // xmap ptr to file offset, take 64 bits pointer from the file for granted when storing in memory
    XBINARYMAP<XQWORD, XQWORD>          seekmaptree;
    GGFINFO&                            info;
  protected:

  private:
    void                                Clean                                 ()
                                        {
                                        }

};


class GGFSTRING
{
  public:
                                        GGFSTRING                             () : string( false)
                                        {
                                          Clean();

                                        }

                                        GGFSTRING                             ( GGFSTRING* gffstring) : string( false)
                                        {
                                          Clean();

                                          this->size    = gffstring->size;


                                          this->string.Resize(this->size);
                                          this->string.Set(gffstring->string.Get(), this->size);
                                          this->helperstring = gffstring->helperstring;
                                        }

                                        GGFSTRING                             ( XCHAR* xchar) : string( false)
                                        {
                                          Clean();


                                          XSTRING str(xchar);
                                          FromXSTRING(&str);

                                          ToXSTRING(&this->helperstring);
                                        }

                                       ~GGFSTRING                             ()
                                        {
                                          Clean();
                                        }

    XDWORD                              Size                                  ()
                                        {
                                          return sizeof(XDWORD) + string.GetSize()*sizeof(XBYTE);
                                        }

    bool                                operator ==                           (GGFSTRING& string)
                                        {
                                          return this->string.Compare(&string.string);
                                        }

    bool                                operator ==                           (XSTRING& string)
                                        {
                                          if(this->helperstring.GetSize() != 0)
                                          {
                                            return this->helperstring.BinaryCompare(string);
                                          }

                                          return *this == string.Get();
                                        }

    bool                                operator ==                           (const XCHAR* string)
                                        {
                                          // compute the length to compare the result
                                          // this is actually faster than comparing until nul character
                                          XDWORD length = 0;
                                          while(*(string+length) != __C('\0'))
                                            {
                                              length++;
                                            }

                                          // alternative from:
                                          // http://fxr.watson.org/fxr/source/libkern/strlen.c?v=DFBSD
                                          //const XCHAR* s;
                                          //for(s = string; *s; ++s);
                                          //const XDWORD length = (s - string);

                                          const XDWORD helpersize = this->helperstring.GetSize();

                                          if(helpersize != 0u)
                                            {
                                            // compare XSTRING with XCHAR
                                              if(helpersize != length)
                                                {
                                                  return false;
                                                }

                                              for(XDWORD j = 0; j < length; j++)
                                                {
                                                  if(this->helperstring[j] != string[j])
                                                    {
                                                      return false;
                                                    }
                                                }

                                                return true;
                                              }

                                          // check, if this is the case the helper string was not created
                                          if(this->string.GetSize() != 0)
                                            {
                                              XDEBUG_PRINTCOLOR(4,__L("GGFSTRING: helper string not created"));
                                            }
                                          // general case

                                          //XSTRING str(string);
                                          //return *this == str;
                                          XDWORD value;
                                          XDWORD oldpos = this->string.GetPosition();
                                          this->string.SetPosition(0);
                                          XDWORD stringsize = this->string.GetSize()/sizeof(XDWORD);

                                          if(stringsize != length)
                                          {
                                            return false;
                                          }

                                          for(XDWORD i = 0; i < stringsize; i++)
                                            {
                                              this->string.Get(value); // mutex lock on each character
                                              if(value != (XDWORD)string[i])
                                                {
                                                  this->string.SetPosition(oldpos);
                                                  return false;
                                                }
                                            }

                                          this->string.SetPosition(oldpos);
                                          return true;
                                        }

    const GGFSTRING&                    operator =                            (GGFSTRING& string)
                                        {
                                          this->size = string.size;
                                          this->string.Resize(string.size);
                                          this->string.Set(string.string.Get(), string.size);
                                          this->helperstring = string.helperstring;
                                          return *this;
                                        }

    XDWORD                              size;
    XBUFFER                             string;

    bool                                FromXSTRING                           (XSTRING* xstring);
    bool                                ToXSTRING                             (XSTRING* xstring);

    bool                                WriteTo                               (XBUFFER& writebuffer);
    bool                                ReadFrom                              (XBUFFER& readbuffer);

    XSTRING                             helperstring;

  protected:




  private:
    void                                Clean                                 ()
                                        {
                                          size = 0;
                                        }
};



class GGFDEFINITIONENTRY
{
  public:
                                        GGFDEFINITIONENTRY                    () : id()
                                        {
                                          Clean();
                                        }

                                       ~GGFDEFINITIONENTRY                    ()
                                        {
                                          Clean();
                                        }

    XDWORD                              Size                                  ()
                                        {
                                          return id.Size() + sizeof(XDWORD);
                                        }

    GGFSTRING                           id;
    XDWORD                              size;

  protected:
  private:
    void                                Clean                                 ()
                                        {
                                          size    = 0;
                                        }
};


class GGFOBJECTDEFINITION
{
  public:
                                        GGFOBJECTDEFINITION                   () : id()
                                        {
                                          Clean();
                                          versionmayor = 1;
                                          versionminor = 0;
                                        }

                                       ~GGFOBJECTDEFINITION                   ()
                                        {
                                          for(XDWORD i = 0; i < entries.GetSize(); i++)
                                            {
                                              delete entries.Get(i);
                                            }

                                          Clean();
                                        }

    void                                SetVersion                            (XDWORD versionmayor, XDWORD versionminor)
                                        {
                                          this->versionmayor = versionmayor;
                                          this->versionminor = versionminor;
                                        }

    XDWORD                              Size                                  ()
                                        {
                                          XDWORD size = 0;

                                          size += sizeof(XDWORD); // version mayor
                                          size += sizeof(XDWORD); // version minor
                                          size += sizeof(XDWORD); // number of items in the definition
                                          size += id.Size(); // definition name

                                          for(XDWORD i = 0; i < entries.GetSize(); i++)
                                            {
                                              size += entries.Get(i)->Size();
                                            }

                                          return size;
                                        }

    void                                AddEntry(GGFDEFINITIONENTRY* entry)
                                        {
                                          entries.Add(entry);
                                        }

    void                                LoadEntry(GGFDEFINITIONENTRY* entry)
                                        {
                                          entries.Add(entry);
                                          maps.Add(entry->id.helperstring, lastindex);
                                          lastindex++;
                                        }

    GGFDEFINITIONENTRY*                 GetEntry(XDWORD index)
                                        {
                                          return entries.Get(index);
                                        }

    GGFDEFINITIONENTRY*                 FastGetEntry(XDWORD index)
                                        {
                                          return entries.FastGet(index);
                                        }

    XDWORD                              GetSize()
                                        {
                                          return entries.GetSize();
                                        }

    XDWORD                              versionmayor;
    XDWORD                              versionminor;
    XDWORD                              items;

    GGFSTRING                           id;

    XBINARYMAP<XSTRING, XDWORD>         maps;

  protected:
    XDWORD                              lastindex;
    XVECTOR<GGFDEFINITIONENTRY* >       entries;

  private:
    void                                Clean                                 ()
                                        {
                                          versionmayor    = 0;
                                          versionminor    = 0;
                                          items           = 0;
                                          lastindex       = 0;
                                        }
};

class GGFOBJECT
{
  public:
                                        GGFOBJECT                             ( GGFINFO& info, GGFOBJECTDEFINITION* definition) : id(), info(info)
                                        {
                                          Clean();

                                          //values.SetIsMulti(true);
                                          this->definition = definition;
                                        }

                                       ~GGFOBJECT                             ()
                                        {
                                          //for(XDWORD i = 0; i < values.GetSize(); i++)
                                          //  {
                                          //    delete values.GetKey(i);
                                          //    delete values.GetElement(i);
                                          //  }

                                          XBINARYMAP<XDWORD, GGFVALUE*>::XITERATOR it;
                                          for(it = indexedvalues.Begin(); it != indexedvalues.End(); it++)
                                          {
                                            delete it.GetElement();
                                          }

                                          Clean();
                                        }


    XDWORD                              Size                                  ()
                                        {
                                          XDWORD size = 0;
                                          size += id.Size();
                                          size += sizeof(XDWORD); // index

                                          size += info.ptrsize;   // object pointer tag
                                          size += sizeof(XDWORD); // number of properties


                                          XBINARYMAP<XDWORD, GGFVALUE*>::XITERATOR it;
                                          for(it = indexedvalues.Begin(); it != indexedvalues.End(); ++it)
                                          {
                                            size += sizeof(XDWORD);
                                            size += it.GetElement()->size;
                                          }

                                          //for(XDWORD i = 0; i < values.GetSize(); i++)
                                          //  {
                                          //    size += sizeof(XDWORD);
                                          //    size += values.GetElement(i)->size;
                                          //  }

                                        return size;
                                        }

    bool                                Add                                   (GGFSTRING* str, GGFVALUE* buffer);
    bool                                Add                                   (XSTRING* str, GGFVALUE* buffer);
    bool                                Add                                   (XCHAR* str, GGFVALUE* buffer);
    GGFVALUE*                           Get                                   (XCHAR* key);
    GGFVALUE*                           Get                                   (XSTRING* key);
    GGFVALUE*                           Get                                   (GGFSTRING* key);

    void                                GetMultiple                           (XCHAR* key, XLIST<GGFVALUE*>& list);
    void                                GetMultiple                           (XSTRING* key, XLIST<GGFVALUE*>& list);
    void                                GetMultiple                           (GGFSTRING* key, XLIST<GGFVALUE*>& list);

    bool                                SetAddInLimit                         (int addinlimit)
                                        {
                                          return true;
                                          //return values.SetAddInLimit(addinlimit);
                                        }

    XDWORD                              index;
    GGFINFO&                            info;
    GGFSTRING                           id;
    XQWORD                              tag;
    //XMAP<GGFSTRING*, GGFVALUE*>         values;
    XBINARYMAP<XDWORD, GGFVALUE*>       indexedvalues;

    GGFOBJECTDEFINITION*                definition;
  protected:



  private:
    void                                Clean                                 ()
                                        {
                                          index       = 0;
                                          tag         = 0;
                                          definition  = NULL;
                                        }
};

class GGFDATA
{
public:
                                        GGFDATA                               () : buffer( false)
                                        {
                                          Clean();
                                        }

                                       ~GGFDATA()
                                        {
                                          Clean();
                                        }

  XDWORD                                Size                                  ()
                                        {
                                          return sizeof(XDWORD) + buffer.GetSize();
                                        }

  XDWORD                                size;
  XBUFFER                               buffer;

protected:
private:
  void                                  Clean                                 ()
                                        {
                                          size = 0;
                                        }
};

// File Loader
class GRPFILEGGF : public GRPFILE3D
{
  public:

                                        GRPFILEGGF                            (GRPSCENE* scn) : GRPFILE3D( scn), seekmap(info)
                                        {
                                          Clean();
                                          header.magic            = 0x2A474746; // 42 followed by 'G' 'G' 'F'
                                          header.versionmayor     = 0;
                                          header.versionminor     = 1;
                                          info.ptrsize            = 4;
                                          definitions.SetIsMulti(false);
                                          objects.SetIsMulti(true);
                                          data.SetIsMulti(true);
                                        }

    virtual                            ~GRPFILEGGF                            ();


    void                                SetVersion                            (XDWORD mayor, XDWORD minor)                        { header.versionmayor = mayor; header.versionminor = minor;   }
    void                                SetPointerSize                        (XDWORD ptrsize)                                    { info.ptrsize = ptrsize;                                     }

    virtual GRPOBJECT*                  Load                                  (XCHAR* file);
    bool                                Write                                 (XCHAR* file);
    bool                                AddDefinition                         (GGFOBJECTDEFINITION* objectdefinition)             { return definitions.Add(objectdefinition);                   }
    bool                                AddObject                             (GGFOBJECT* object)
                                        {
                                          if(!objects.Add(object)) return false;
                                          return seekmap.Add((XQWORD)(object->tag), seekmap.GetSize());
                                        }

    XDWORD                              AddData                               (GGFDATA* data)
                                        {
                                          XDWORD index = this->data.GetSize();
                                          this->data.Add(data);
                                          return index;
                                        }

    XVECTOR<GGFOBJECTDEFINITION*>*      GetDefinitions                        ()                                                  { return &definitions;                                        }

    GGFHEADER                           header;
    GGFINFO                             info;
    GGFSEEKMAP                          seekmap;

    XVECTOR<GGFOBJECT*>                 objects;
    XVECTOR<GGFDATA*>                   data;

  protected:
    XFILE*                              xfile;


    XVECTOR<GGFOBJECTDEFINITION*>       definitions;
  private:

    // Reading functions

    bool                                LoadHeader                            ();
    bool                                LoadInfo                              ();
    bool                                LoadSeekMap                           (XBUFFER& readbuffer);
    bool                                LoadObjectDefinitions                 (XBUFFER& readbuffer);
    bool                                LoadObjects                           (XBUFFER& readbuffer);
    bool                                LoadData                              (XBUFFER& readbuffer);

    bool                                ReadGGFDATA                           (GGFDATA& data, XBUFFER& readvuffer);
    bool                                ReadGGFSTRING                         (GGFSTRING& ggfstring, XBUFFER& readbuffer);
    bool                                ReadGGFOBJECTDEFINITION               (GGFOBJECTDEFINITION & ggfobjectdefinition, XBUFFER& readbuffer);
    bool                                ReadGGFOBJECT                         (GGFOBJECT& ggfobject, GGFOBJECTDEFINITION& ggfobjectdefinition, XBUFFER& readbuffer);


    // Writing functions
    bool                                WriteHeader                           ();
    bool                                WriteInfo                             ();
    bool                                WriteSeekMap                          (XBUFFER& writebuffer);
    bool                                WriteObjectDefinitions                (XBUFFER& writebuffer);
    bool                                WriteObjects                          (XBUFFER& writebuffer);
    bool                                WriteData                             (XBUFFER& writebuffer);

    bool                                WriteGGFDATA                          (GGFDATA& data, XBUFFER& writebuffer);
    bool                                WriteGGFSTRING                        (GGFSTRING& ggfstring, XBUFFER & writebuffer);
    bool                                WriteGGFOBJECTDEFINITION              (GGFOBJECTDEFINITION & ggfobjectdefinition, XBUFFER & writebuffer);
    bool                                WriteGGFOBJECT                        (GGFOBJECT& ggfobject, GGFOBJECTDEFINITION& ggfobjectdefinition, XDWORD index, XBUFFER & writebuffer);


    XDWORD                              GetDefinitionsSize();
    XDWORD                              GetObjectsSize();
    XDWORD                              GetDataSize();

    void                                Clean                                 ()
                                        {
                                          xfile                 = NULL;
                                          header.versionmayor   = 0;
                                          header.versionminor   = 0;
                                          info.ptrsize          = 0;
                                        }
};


/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/

#endif

