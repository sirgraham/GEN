
/*------------------------------------------------------------------------------------------
//	GRPFILEGGF.CPP
//	
//	loads a ggf file
//   
//	Author						: Imanol Celaya Ruiz de Alegria
//	Date Of Creation	: 13/11/2015 9:31:05
//	Last Modification	:	
//	
//	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/
	
#include "GRPFileGGF.h"
	
#include "HashCRC32.h"

#include "XFile.h"
#include "XDebug.h"

#include "XMemory.h"


/*---- GENERAL VARIABLE ------------------------------------------------------------------*/
	
	
/*---- CLASS MEMBERS ---------------------------------------------------------------------*/



/*-------------------------------------------------------------------
//	GGFSTRING::FromXSTRING
*/	
/**	
//	
//	sets the content form a xstring
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			13/11/2015 16:32:01
//	
//	@return 			bool : 
//
//  @param				xstring : 
*/
/*-----------------------------------------------------------------*/
bool GGFSTRING::FromXSTRING(XSTRING* xstring)
{
	xstring->ConvertToXBUFFER(this->string);
	this->helperstring = *xstring;

	size = xstring->GetSize()*sizeof(XDWORD);

	return true;
}





/*-------------------------------------------------------------------
//	GGFSTRING::ToXSTRING
*/	
/**	
//	
//	sets the content to a xstring
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			13/11/2015 16:32:19
//	
//	@return 			bool : 
//
//  @param				xstring : 
*/
/*-----------------------------------------------------------------*/
bool GGFSTRING::ToXSTRING(XSTRING* xstring)
{
	// doesn't work, XSTRING can't really get a string from a normalized one?
	return xstring->ConvertFromXBUFFER(this->string);
}





/*-------------------------------------------------------------------
//	GGFSTRING::WriteTo
*/	
/**	
//	
//	writes a string to a buffer
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			20/11/2015 16:30:15
//	
//	@return 			bool : 
//
//  @param				writebuffer : 
*/
/*-----------------------------------------------------------------*/
bool GGFSTRING::WriteTo(XBUFFER& writebuffer)
{
	writebuffer.Set((XBYTE*)&size, sizeof(size));
	return writebuffer.Set(string.Get(), string.GetSize()*sizeof(XBYTE));
}




/*-------------------------------------------------------------------
//	GGFSTRING::ReadFrom
*/	
/**	
//	
//	reads a ggfstring from a buffer
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			20/11/2015 16:30:31
//	
//	@return 			bool : 
//
//  @param				readbuffer : 
*/
/*-----------------------------------------------------------------*/
bool GGFSTRING::ReadFrom(XBUFFER& readbuffer)
{
	readbuffer.Get((XBYTE*)&size, sizeof(size), 0);
	string.Resize(size);
	readbuffer.Get(string.Get(), size, 4);

	ToXSTRING(&this->helperstring);
	return true;
}



/*-------------------------------------------------------------------
//	GGFOBJECT::Add
*/	
/**	
//	
//	adds a pair to an object
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			13/11/2015 16:32:39
//	
//	@return 			bool : 
//
//  @param				key : 
//  @param				buffer : 
*/
/*-----------------------------------------------------------------*/
bool GGFOBJECT::Add(GGFSTRING* key, GGFVALUE* value)
{
	GGFVALUE* buffer = new GGFVALUE();
	if(!buffer)
		{
			return false;
		}

	if(!buffer->buffer.Resize(value->size)) return false;
	if(!buffer->buffer.Set(value->buffer.Get(), value->size)) return false;

	buffer->size = value->size;

	//GGFSTRING* str = new GGFSTRING( key);
	//if(!str)
	//	{
	//		return false;
	//	}

	if(definition)
	{
		// maps don't have the entries added in reality at this point
		XDWORD index = definition->maps.Get(key->helperstring);
		indexedvalues.Add(index, buffer);
	}


	return true;
	//GGFSTRING* str = new GGFSTRING( key);
	//if(!str)
	//	{
	//		return false;
	//	}
	//return values.Add(str, buffer);
}





/*-------------------------------------------------------------------
//	GGFOBJECT::Add
*/	
/**	
//	
//	adds a value to an object
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			17/11/2015 11:12:21
//	
//	@return 			bool : 
//
//  @param				str : 
//  @param				buffer : 
*/
/*-----------------------------------------------------------------*/
bool GGFOBJECT::Add(XSTRING*	str, GGFVALUE* buffer)
{
	GGFSTRING key;

	if(!key.FromXSTRING(str)) return false;

	return Add(&key, buffer);
}





/*-------------------------------------------------------------------
//	GGFOBJECT::Add
*/	
/**	
//	
//	adds a value to an object
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			23/11/2015 9:46:07
//	
//	@return 			bool : 
//
//  @param				str : 
//  @param				buffer : 
*/
/*-----------------------------------------------------------------*/
bool GGFOBJECT::Add(XCHAR*	str, GGFVALUE* buffer)
{
	XSTRING string(str);
	return this->Add(&string, buffer);
}




/*-------------------------------------------------------------------
//	GGFOBJECT::Get
*/	
/**	
//	
//	gets an object form an xchar key
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			24/11/2015 14:30:08
//	
//	@return 			GGFVALUE* : 
//
//  @param				key : 
*/
/*-----------------------------------------------------------------*/
GGFVALUE* GGFOBJECT::Get(XCHAR* key)
{
	//for(XDWORD i = 0; i < values.GetSize(); i++)
	//	{
	//		if(*values.keys.FastGet(i) == key)
	//			{
	//				return values.elements.FastGet(i);
	//			}
	//	}
	//
	//return NULL;

	XSTRING str(key);
	return Get(&str);
}


/*-------------------------------------------------------------------
//	GGFOBJECT::Get
*/	
/**	
//	
//	gets the value from a key
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			13/11/2015 16:32:59
//	
//	@return 			GGFVALUE* : 
//
//  @param				key : 
*/
/*-----------------------------------------------------------------*/
GGFVALUE* GGFOBJECT::Get(XSTRING* key)
{
	//for(XDWORD i = 0; i < values.GetSize(); i++)
	//	{
	//		if(*values.keys.FastGet(i) == key->Get())
	//			{
	//				return values.elements.FastGet(i);
	//			}
	//	}

	XDWORD index = definition->maps.Get(*key);
	GGFVALUE* value = indexedvalues.Get(index);
	return value;

	return NULL;

}





/*-------------------------------------------------------------------
//	GGFOBJECT::Get
*/	
/**	
//	
//	gets a value from a GFFSTRING
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			16/11/2015 13:56:16
//	
//	@return 			GGFVALUE* : 
//
//  @param				key : 
*/
/*-----------------------------------------------------------------*/
GGFVALUE* GGFOBJECT::Get(GGFSTRING* key)
{
	//for(XDWORD i = 0; i < values.GetSize(); i++)
	//	{
	//		if(*values.keys.FastGet(i) == *key)
	//			{
	//				return values.elements.FastGet(i);
	//			}
	//	}

	//XDWORD index = definition->maps(
	XDWORD index = definition->maps.Get(key->helperstring);
	GGFVALUE* value = indexedvalues.Get(index);
	return value;
	return NULL;
}

void GGFOBJECT::GetMultiple(XCHAR* key, XLIST<GGFVALUE*>& list)
{
	XSTRING str(key);
	GetMultiple(&str, list);
}

void GGFOBJECT::GetMultiple(XSTRING* key, XLIST<GGFVALUE*>& list)
{
	XDWORD index = definition->maps.Get(*key);
	indexedvalues.GetMultiple(index, list);
}

void GGFOBJECT::GetMultiple(GGFSTRING* key, XLIST<GGFVALUE*>& list)
{
	XDWORD index = definition->maps.Get(key->helperstring);
	indexedvalues.GetMultiple(index, list);
}



/*-------------------------------------------------------------------
//	GRPFILEGGF::~GRPFILEGGF
*/	
/**	
//	
//	 Class Destructor GRPFILEGGF
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			16/11/2015 13:56:24
//	
*/
/*-----------------------------------------------------------------*/
GRPFILEGGF::~GRPFILEGGF()
{
	if(xfile)
		{
			xfactory->Delete_File(xfile);
		}

	for(XDWORD i = 0; i < definitions.GetSize(); i++)
		{
			delete definitions.FastGet(i);
		}

	for(XDWORD i = 0; i < objects.GetSize(); i++)
		{
			delete objects.FastGet(i);
		}

	for(XDWORD i = 0; i < data.GetSize(); i++)
		{
			delete data.FastGet(i);
		}


	Clean();
}


/*-------------------------------------------------------------------
//	GRPFILEGGF::Load
*/	
/**	
//	
//	loads a ggf file
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			13/11/2015 9:36:02
//	
//	@return 			GRPOBJECT* : 
//
//  @param				file : 
*/
/*-----------------------------------------------------------------*/
GRPOBJECT* GRPFILEGGF::Load(XCHAR* file)
{
	xfile = xfactory->Create_File();

	if(!xfile)
		{
			return NULL;
		}

	XPATH			xpath(file);

	if(!xfile->Open(xpath))
		{
			return NULL;
		}

	if(!LoadHeader())
		{
			xfile->Close();
			return NULL;
		}

	if(!LoadInfo())
		{
			xfile->Close();
			return NULL;
		}

	XTIMER* ggftimer = xfactory->CreateTimer();
	ggftimer->Reset();

	// read all of the data into an XBUFFER
	XBUFFER readbuffer( false);
	readbuffer.Resize(info.datasize);


	xfile->Read(readbuffer.Get(), info.datasize);

	XDEBUG_PRINTCOLOR(3,__L("\n\n  GGF File loaded in %lld \n\n"),ggftimer->GetMeasureMilliSeconds());
	ggftimer->Reset();

	HASHCRC32 hash;
	hash.Do(readbuffer.Get(), readbuffer.GetSize());

	if(info.hash != hash.GetResultCRC32())
		{
			XDEBUG_PRINTCOLOR(4,__L("\t\tFile Hash Doesn't Match!"));
			xfile->Close();
			return NULL;
		}

	XDEBUG_PRINTCOLOR(3,__L("\n\n  GGF Hash computed in %lld \n\n"),ggftimer->GetMeasureMilliSeconds());
	ggftimer->Reset();

	if(!LoadSeekMap(readbuffer))
		{
			xfactory->DeleteTimer(ggftimer);
			xfile->Close();
			return NULL;
		}
	
	XDEBUG_PRINTCOLOR(3,__L("\n\n  GGF SeekMap loaded in %lld \n\n"),ggftimer->GetMeasureMilliSeconds());
	ggftimer->Reset();

	if(!LoadObjectDefinitions(readbuffer))
		{
			xfactory->DeleteTimer(ggftimer);
			xfile->Close();
			return NULL;
		}
	
	XDEBUG_PRINTCOLOR(3,__L("\n\n  GGF ObjectDefinitions loaded in %lld \n\n"),ggftimer->GetMeasureMilliSeconds());
	ggftimer->Reset();

	if(!LoadObjects(readbuffer))
		{
			xfactory->DeleteTimer(ggftimer);
			xfile->Close();
			return NULL;
		}

	XDEBUG_PRINTCOLOR(3,__L("\n\n  GGF Objects loaded in %lld \n\n"),ggftimer->GetMeasureMilliSeconds());
	ggftimer->Reset();

	if(!LoadData(readbuffer))
		{
			xfactory->DeleteTimer(ggftimer);
			xfile->Close();
			return NULL;
		}

	XDEBUG_PRINTCOLOR(3,__L("\n\n  GGF Data loaded in %lld \n\n"),ggftimer->GetMeasureMilliSeconds());
	ggftimer->Reset();

	// actual scene loading work begins
	xfactory->DeleteTimer(ggftimer);
	xfile->Close();

	return NULL;
}


bool GRPFILEGGF::Write(XCHAR* file)
{
	xfile = xfactory->Create_File();


	if(!xfile)
		{
			return false;
		}

	XPATH			xpath(file);

	if(!xfile->Create(xpath))
		{
			return false;
		}

	if(!WriteHeader())
		{
			xfile->Close();
			return false;
		}
	
	info.datasize = seekmap.Size() + GetDefinitionsSize() + GetObjectsSize() + GetDataSize();

	if(!WriteInfo())
		{
			xfile->Close();
			return false;
		}

	XBUFFER writebuffer;

	writebuffer.Resize(info.datasize);

	if(!WriteSeekMap(writebuffer))
		{
			xfile->Close();
			return false;
		}

	if(!WriteObjectDefinitions(writebuffer))
		{
			xfile->Close();
			return false;
		}

	if(!WriteObjects(writebuffer))
		{
			xfile->Close();
			return false;
		}

	if(!WriteData(writebuffer))
		{
			xfile->Close();
			return false;
		}
	
	xfile->Write(writebuffer);

	// write the data hash in here
	HASHCRC32 hash;

	hash.Do(writebuffer.Get(), writebuffer.GetSize());

	info.hash = hash.GetResultCRC32();

	if(!WriteInfo())
		{
			xfile->Close();
			return false;
		}

	xfile->Close();

	return true;
}


/*-------------------------------------------------------------------
//	GRPFILEGGF::LoadHeader
*/	
/**	
//	
//	loads the ggf header
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			13/11/2015 9:36:12
//	
*/
/*-----------------------------------------------------------------*/
bool GRPFILEGGF::LoadHeader()
{
	XBUFFER readbuffer;

	// magic number
	if(!xfile->SetPosition(0*sizeof(XDWORD))) return false;
	if(!readbuffer.Resize(sizeof(XDWORD))) return false;
	if(!xfile->Read(readbuffer.Get(), sizeof(XDWORD))) return false;
	if(!readbuffer.Extract(header.magic, 0)) return false;

	// mayor version
	if(!xfile->SetPosition(1*sizeof(XDWORD))) return false;
	if(!readbuffer.Resize(sizeof(XDWORD))) return false;
	if(!xfile->Read(readbuffer.Get(), sizeof(XDWORD))) return false;
	if(!readbuffer.Extract(header.versionmayor, 0)) return false;

	// minor version
	if(!xfile->SetPosition(2*sizeof(XDWORD))) return false;
	if(!readbuffer.Resize(sizeof(XDWORD))) return false;
	if(!xfile->Read(readbuffer.Get(), sizeof(XDWORD))) return false;
	if(!readbuffer.Extract(header.versionminor, 0)) return false;

	return true;
}



/*-------------------------------------------------------------------
//	GRPFILEGGF::LoadInfo
*/	
/**	
//	
//	loads the ggf info
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			13/11/2015 9:36:24
//	
*/
/*-----------------------------------------------------------------*/
bool GRPFILEGGF::LoadInfo()
{
	XBUFFER readbuffer;
	
	XDWORD position = header.Size();

	// pointer size in bytes
	if(!xfile->SetPosition(position)) return false;
	if(!readbuffer.Resize(sizeof(XDWORD))) return false;
	if(!xfile->Read(readbuffer.Get(), sizeof(XDWORD))) return false;
	if(!readbuffer.Extract(info.ptrsize, 0)) return false;

	position += sizeof(XDWORD);

	// pointer size in bytes
	if(!xfile->SetPosition(position)) return false;
	if(!readbuffer.Resize(sizeof(XDWORD))) return false;
	if(!xfile->Read(readbuffer.Get(), sizeof(XDWORD))) return false;
	if(!readbuffer.Extract(info.datasize, 0)) return false;

	position += sizeof(XDWORD);

	// pointer size in bytes
	if(!xfile->SetPosition(position)) return false;
	if(!readbuffer.Resize(sizeof(XDWORD))) return false;
	if(!xfile->Read(readbuffer.Get(), sizeof(XDWORD))) return false;
	if(!readbuffer.Extract(info.hash, 0)) return false;

	return true;
}





/*-------------------------------------------------------------------
//	GRPFILEGGF::LoadSeekMap
*/	
/**	
//	
//	loads the seek map
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			13/11/2015 10:21:37
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool GRPFILEGGF::LoadSeekMap(XBUFFER& readbuffer)
{
	XDWORD seekmapentries;

	// read unmber of entries on the seekmap
	readbuffer.Get((XBYTE*)&seekmapentries, sizeof(seekmapentries));

	// need to reserve space for the seekmap in here
	//seekmap.seekmap.SetAddInLimit(seekmapentries+1);

	// loop on the number of entries
	for(XDWORD i = 0; i < seekmapentries; i++)
		{
			XQWORD id = 0;
			XQWORD offset = 0;

			// id of the entry
			readbuffer.Get((XBYTE*)&id, info.ptrsize);

			// read the offset
			readbuffer.Get((XBYTE*)&offset, sizeof(offset));

			// add the info into the seekmap
			if(!seekmap.Add(id, offset)) return false;
		}

	return true;
}



/*-------------------------------------------------------------------
//	GRPFILEGGF::LoadObjectDefinitions
*/	
/**	
//	
//	loads the object definitions list
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			13/11/2015 12:18:40
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool GRPFILEGGF::LoadObjectDefinitions(XBUFFER& readbuffer)
{
	XDWORD definitionsnumber = 0;

	// number of object definitions
	readbuffer.Get((XBYTE*)&definitionsnumber, sizeof(definitionsnumber));

	// load the definitions
	for(XDWORD i = 0; i < definitionsnumber; i++)
		{
			GGFOBJECTDEFINITION* definition = new GGFOBJECTDEFINITION();
			if(!definition) return false;

			if(!ReadGGFOBJECTDEFINITION(*definition, readbuffer))
				{
					delete definition;
					return false;
				}

			if(!definitions.Add(definition)) return false;
		}

	return true;
}



/*-------------------------------------------------------------------
//	GRPFILEGGF::LoadObjects
*/	
/**	
//	
//	Loads all the objects from a file
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			16/11/2015 16:15:28
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool GRPFILEGGF::LoadObjects(XBUFFER& readbuffer)
{
	// need position of current cursor
	XDWORD objectsnum = 0;

	// number of object definitions
	readbuffer.Get((XBYTE*)&objectsnum, sizeof(objectsnum));

	objects.SetAddInLimit(objectsnum+1);
	objects.Resize(objectsnum);

	for(XDWORD i = 0; i < objectsnum; i++)
		{
			GGFSTRING str;

			ReadGGFSTRING(str, readbuffer);

			// read definition index
			XDWORD index = 0;
			readbuffer.Get((XBYTE*)&index, sizeof(index));

			GGFOBJECTDEFINITION* definition;
			definition = definitions.Get(index);

			if(!definition)
				{
					XSTRING string;
					str.ToXSTRING(&string);
					XDEBUG_PRINTCOLOR(4,__L("ERROR - Object definition not found: %s"), string.Get());
					return false;
				}

			GGFOBJECT* object = new GGFOBJECT( this->info, definition);
			object->index = index;
			if(!object)
				{
					return false;
				}

			object->id = str;

			// read the object
			if(!ReadGGFOBJECT(*object, *definition, readbuffer)) return false;

			objects.Set(i, object);
			//objects.Add(object);
		}

	return true;
}




/*-------------------------------------------------------------------
//	GRPFILEGGF::LoadData
*/	
/**	
//	
//	loads the data area
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			20/11/2015 13:15:04
//	
//	@return 			bool : 
//
//  @param				readbuffer : 
*/
/*-----------------------------------------------------------------*/
bool GRPFILEGGF::LoadData(XBUFFER& readbuffer)
{
	XDWORD datanum = 0;
	readbuffer.Get((XBYTE*)&datanum, sizeof(datanum));

	data.SetAddInLimit(datanum);
	data.Resize(datanum);

	for(XDWORD i = 0; i < datanum; i++)
		{
			GGFDATA * dataptr = new GGFDATA();

			if(!dataptr)
				{
					return false;
				}

			if(!ReadGGFDATA(*dataptr, readbuffer))			{ delete dataptr; return false; }
			if(!data.Set(i, dataptr))								{ delete dataptr; return false; }
			//if(!data.Add(dataptr))											{ delete dataptr; return false; }
		}

	return true;
}


/*-------------------------------------------------------------------
//	GRPFILEGGF::ReadGGFDATA
*/	
/**	
//	
//	reads a data entry
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			20/11/2015 12:59:22
//	
//	@return 			bool : 
//
//  @param				data : 
//  @param				readvuffer : 
*/
/*-----------------------------------------------------------------*/
bool GRPFILEGGF::ReadGGFDATA(GGFDATA& data, XBUFFER& readbuffer)
{
	readbuffer.Get((XBYTE*)&data.size, sizeof(data.size));
	data.buffer.Resize(data.size);
	readbuffer.Get(data.buffer.Get(), data.size);

	return true;
}


/*-------------------------------------------------------------------
//	GRPFILEGGF::ReadGGFSTRING
*/	
/**	
//	
//	reads a ggf string
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			13/11/2015 10:32:48
//	
//	@return 			bool : 
//
//  @param				ggfstring : 
*/
/*-----------------------------------------------------------------*/
bool GRPFILEGGF::ReadGGFSTRING(GGFSTRING& ggfstring, XBUFFER& readbuffer)
{
	// string character count
	readbuffer.Get((XBYTE*)&ggfstring.size, sizeof(ggfstring.size));

	// string data
	ggfstring.string.Resize(ggfstring.size);
	readbuffer.Get(ggfstring.string.Get(), ggfstring.size);

	ggfstring.ToXSTRING(&ggfstring.helperstring);

	ggfstring.string.Delete();

	return true;
}





/*-------------------------------------------------------------------
//	GRPFILEGGF::ReadGGFOBJECTDEFINITION
*/	
/**	
//	
//	reads a single object definition
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			13/11/2015 12:18:03
//	
//	@return 			bool : 
//
//  @param				ggfobjectdefinition : 
//  @param				position : 
*/
/*-----------------------------------------------------------------*/
bool GRPFILEGGF::ReadGGFOBJECTDEFINITION(GGFOBJECTDEFINITION & ggfobjectdefinition, XBUFFER& readbuffer)
{
	// mayor version
	readbuffer.Get((XBYTE*)&ggfobjectdefinition.versionmayor, sizeof(ggfobjectdefinition.versionmayor));

	// minor version
	readbuffer.Get((XBYTE*)&ggfobjectdefinition.versionminor, sizeof(ggfobjectdefinition.versionminor));

	// definition number of items in the definition
	readbuffer.Get((XBYTE*)&ggfobjectdefinition.items, sizeof(ggfobjectdefinition.items));

	// read the id string
	ReadGGFSTRING(ggfobjectdefinition.id, readbuffer);


	// read now the list of object definitions
	for(XDWORD i = 0; i < ggfobjectdefinition.items; i++)
		{
			GGFDEFINITIONENTRY* entry = new GGFDEFINITIONENTRY();
			if(!entry)
				{
					return false;
				}
		
			// name of the entry
			if(!ReadGGFSTRING(entry->id, readbuffer))																			{ delete entry; return false; }

			// size in bytes of the entry
			if(!readbuffer.Get((XBYTE*)&(entry->size), sizeof(entry->size)))							{ delete entry; return false; }

			ggfobjectdefinition.LoadEntry(entry);
		}

	return true;
}




/*-------------------------------------------------------------------
//	GRPFILEGGF::ReadGGFOBJECT
*/	
/**	
//	
//	reads an object form its definition
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			13/11/2015 16:53:28
//	
//	@return 			bool : 
//
//  @param				ggfobject : 
//  @param				ggfobjectdefinition : 
//  @param				position : 
*/
/*-----------------------------------------------------------------*/
bool GRPFILEGGF::ReadGGFOBJECT(GGFOBJECT& ggfobject, GGFOBJECTDEFINITION& ggfobjectdefinition, XBUFFER& readbuffer)
{
	// read the number tag
	readbuffer.Get((XBYTE*)&ggfobject.tag, info.ptrsize);

	// for the number of entries inside
	XDWORD entries;
	readbuffer.Get((XBYTE*)&entries, sizeof(entries));
	
	ggfobject.SetAddInLimit(entries+1);

	for(XDWORD i = 0; i < entries; i++)
		{
			// READ THE PROPERTY INDEX INSTEAD OF THE NAME
			XDWORD index = 0;
			// read the number tag
			readbuffer.Get((XBYTE*)&index, sizeof(index));

			// find property size in the definition
			GGFDEFINITIONENTRY* definition = ggfobjectdefinition.GetEntry(index);
		
			if(definition)
				{
					// read appropiate size
					XDWORD sizetoread = definition->size;
					
					GGFVALUE value;

					value.buffer.Resize(sizetoread);
					//value.buffer.Put(readbuffer.Get(), sizetoread); // readbuffer is not moved as it should
					readbuffer.Get(value.buffer.Get(), sizetoread);
					//readbuffer.SetPosition(readbuffer.GetPosition() + sizetoread);
					value.size = sizetoread;
					// get value from readbuffer
					//readbuffer.Get(value.buffer.Get(), sizetoread);

					// store it in the object
					ggfobject.Add(&(definition->id), &value); // around 200MB go into here
				}
			else
				{
					// throw en error of reading a property with no definition
					XSTRING str;
					ggfobject.id.ToXSTRING(&str);
					XDEBUG_PRINTCOLOR(4,__L("ERROR - The property was not found in the definitions Object: %s     Index: %d"), ggfobject.id.helperstring.Get() , index);
					return false;
				}
		}

	return true;
}


/*-------------------------------------------------------------------
//	GRPFILEGGF::WriteHeader
*/	
/**	
//	
//	writes the file header
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			13/11/2015 13:08:49
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool GRPFILEGGF::WriteHeader()
{
	XBUFFER writebuffer;

	// magic number
	if(!xfile->SetPosition(0*sizeof(XDWORD))) return false;
	if(!writebuffer.Resize(sizeof(XDWORD))) return false;
	if(!writebuffer.Insert(header.magic)) return false;
	if(!xfile->Write(writebuffer.Get(), sizeof(XDWORD))) return false;

	// mayor version
	if(!xfile->SetPosition(1*sizeof(XDWORD))) return false;
	if(!writebuffer.Resize(sizeof(XDWORD))) return false;
	if(!writebuffer.Insert(header.versionmayor)) return false;
	if(!xfile->Write(writebuffer.Get(), sizeof(XDWORD))) return false;

	// minor version
	if(!xfile->SetPosition(2*sizeof(XDWORD))) return false;
	if(!writebuffer.Resize(sizeof(XDWORD))) return false;
	if(!writebuffer.Insert(header.versionminor)) return false;
	if(!xfile->Write(writebuffer.Get(), sizeof(XDWORD))) return false;

	return true;
}



/*-------------------------------------------------------------------
//	GRPFILEGGF::WriteInfo
*/	
/**	
//	
//	writes the file info
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			13/11/2015 13:08:56
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool GRPFILEGGF::WriteInfo()
{
	XBUFFER writebuffer;
	XDWORD position = header.Size();

	// pointer size
	if(!xfile->SetPosition(position)) return false;
	if(!writebuffer.Resize(sizeof(XDWORD))) return false;
	if(!writebuffer.Insert(info.ptrsize)) return false;
	if(!xfile->Write(writebuffer.Get(), sizeof(XDWORD))) return false;

	position += sizeof(XDWORD);

	// file size
	if(!xfile->SetPosition(position)) return false;
	if(!writebuffer.Resize(sizeof(XDWORD))) return false;
	if(!writebuffer.Insert(info.datasize)) return false;
	if(!xfile->Write(writebuffer.Get(), sizeof(XDWORD))) return false;

	position += sizeof(XDWORD);

	// hash
	if(!xfile->SetPosition(position)) return false;
	if(!writebuffer.Resize(sizeof(XDWORD))) return false;
	if(!writebuffer.Insert(info.hash)) return false;
	if(!xfile->Write(writebuffer.Get(), sizeof(XDWORD))) return false;

	return true;
}



/*-------------------------------------------------------------------
//	GRPFILEGGF::WriteSeekMap
*/	
/**	
//	
//	writes the seek map
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			13/11/2015 13:09:05
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool GRPFILEGGF::WriteSeekMap(XBUFFER & writebuffer)
{
	
	//   std::stringstream stm;
	//   stm << "digraph {\n";
	//	PreOrder(stm, seekmap.seekmaptree.GetRoot());
	//   stm << "}\n";
	//
	//
	//   std::ofstream out("out.gv");
	//   out << stm.str() << std::endl;
	//   out.close();
	//   system("dot -Tpng out.gv -o out.png");

	// number of seekmap entries
	XDWORD value = seekmap.GetSize();
	//XDWORD value = seekmap.seekmaptree.GetSize();

	writebuffer.Set((XBYTE*)&value, sizeof(XDWORD));
	XBINARYMAP<XQWORD, XQWORD>::XITERATOR it;
	for(it = seekmap.seekmaptree.Begin(); it != seekmap.seekmaptree.End(); it++)
	{
		XQWORD id = 0;
		XQWORD offset = 0;
	
		id = it.GetKey();
		offset = it.GetElement();
	
		XBYTE* toextract;
	
		// write the id
		toextract = (XBYTE*)&id; // get the actual position to read into
		XDWORD delta = sizeof(id) - info.ptrsize;
		writebuffer.Set((XBYTE*)toextract, info.ptrsize);
	
		// write the offset
		toextract = (XBYTE*)&offset;
		writebuffer.Set(toextract, sizeof(XQWORD));
	}

	//// loop on the number of entries
	//for(XDWORD i = 0; i < seekmap.GetSize(); i++)
	//	{
	//		XQWORD id = 0;
	//		XQWORD offset = 0;
	//
	//		id = seekmap.seekmap.GetKey(i);
	//		offset = seekmap.seekmap.GetElement(i);
	//
	//		XBYTE* toextract;
	//
	//		// write the id
	//		toextract = (XBYTE*)&id; // get the actual position to read into
	//		XDWORD delta = sizeof(id) - info.ptrsize;
	//		writebuffer.Put(toextract, info.ptrsize);
	//
	//		// write the offset
	//		toextract = (XBYTE*)&offset;
	//		writebuffer.Put(toextract, sizeof(XQWORD));
	//	}

	return true;
}





/*-------------------------------------------------------------------
//	GRPFILEGGF::WriteObjectDefinitions
*/	
/**	
//	
//	writes the object definitions
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			13/11/2015 13:09:57
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool GRPFILEGGF::WriteObjectDefinitions(XBUFFER & writebuffer)
{
	XDWORD value = definitions.GetSize();
	writebuffer.Set((XBYTE*)&value, sizeof(XDWORD));
	
	// write the definitions
	for(XDWORD i = 0; i < definitions.GetSize(); i++)
		{
			GGFOBJECTDEFINITION* definition = definitions.Get(i);
	
			if(!WriteGGFOBJECTDEFINITION(*definition, writebuffer))
				{
					return false;
				}
		}

	return true;
}




/*-------------------------------------------------------------------
//	GRPFILEGGF::WriteObjects
*/	
/**	
//	
//	writes the objects that have been stored
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			16/11/2015 14:17:08
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool GRPFILEGGF::WriteObjects(XBUFFER & writebuffer)
{
	XDWORD objectsnum = objects.GetSize();

	writebuffer.Set((XBYTE*)&objectsnum, sizeof(objectsnum));

	for(XDWORD i = 0; i < objectsnum; i++)
		{
			// find corresponding definitions
			GGFOBJECTDEFINITION* definition = NULL;
			XDWORD definitionindex = 0;
			for(XDWORD j = 0; j < definitions.GetSize(); j++)
				{
					if(objects.FastGet(i)->id == definitions.FastGet(j)->id)
						{
							definition = definitions.FastGet(j);
							definitionindex = j;
							break;
						}
				}
			if(!definition) return false;

			// write the object
			if(!WriteGGFOBJECT(*objects.Get(i), *definition, definitionindex, writebuffer)) return false;
		}

	return true;
}




/*-------------------------------------------------------------------
//	GRPFILEGGF::WriteData
*/	
/**	
//	
//	writes the data area
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			20/11/2015 13:09:48
//	
//	@return 			bool : 
//
//  @param				writebuffer : 
*/
/*-----------------------------------------------------------------*/
bool GRPFILEGGF::WriteData(XBUFFER& writebuffer)
{
	XDWORD value = data.GetSize();
	writebuffer.Set((XBYTE*)&value, sizeof(value));

	for(XDWORD i = 0; i < data.GetSize(); i++)
		{
			WriteGGFDATA(*data.FastGet(i), writebuffer);
		}

	return true;
}



/*-------------------------------------------------------------------
//	GRPFILEGGF::WriteGGFDATA
*/	
/**	
//	
//	reads a data entry
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			20/11/2015 13:01:02
//	
//	@return 			bool : 
//
//  @param				data : 
//  @param				writebuffer : 
*/
/*-----------------------------------------------------------------*/
bool GRPFILEGGF::WriteGGFDATA(GGFDATA& data, XBUFFER& writebuffer)
{
	// size od the data buffer
	XDWORD size = data.buffer.GetSize();
	writebuffer.Set((XBYTE*)&size, sizeof(XDWORD));

	// the data buffer
	writebuffer.Set(data.buffer.Get(), size);

	return true;
}



/*-------------------------------------------------------------------
//	GRPFILEGGF::WriteGGFSTRING
*/	
/**	
//	
//	writes a ggfstring
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			13/11/2015 13:09:15
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool GRPFILEGGF::WriteGGFSTRING(GGFSTRING& ggfstring, XBUFFER & writebuffer)
{
	// size of the string in characters
	writebuffer.Set((XBYTE*)&ggfstring.size, sizeof(ggfstring.size));

	// the string itself
	writebuffer.Set(ggfstring.string.Get(), ggfstring.string.GetSize()*sizeof(XBYTE));

	return true;
}





/*-------------------------------------------------------------------
//	GRPFILEGGF::WriteGGFOBJECTDEFINITION
*/	
/**	
//	
//	writes an object definition
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			13/11/2015 13:11:02
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool GRPFILEGGF::WriteGGFOBJECTDEFINITION(GGFOBJECTDEFINITION & ggfobjectdefinition, XBUFFER & writebuffer)
{
	writebuffer.Set((XBYTE*)&ggfobjectdefinition.versionmayor, sizeof(ggfobjectdefinition.versionmayor));

	// minor version
	writebuffer.Set((XBYTE*)&ggfobjectdefinition.versionminor, sizeof(ggfobjectdefinition.versionminor));
	
	// definition number of items in the definition
	XDWORD value = ggfobjectdefinition.GetSize();
	writebuffer.Set((XBYTE*)&value, sizeof(value));
	
	// read the id string
	WriteGGFSTRING(ggfobjectdefinition.id, writebuffer);
	
	
	// read now the list of object definitions
	for(XDWORD i = 0; i < ggfobjectdefinition.GetSize(); i++)
		{
			GGFDEFINITIONENTRY* entry = ggfobjectdefinition.GetEntry(i);
		
			// name of the entry
			if(!WriteGGFSTRING(entry->id, writebuffer))														{ delete entry; return false; }
	
			// size in bytes of the entry
			if(!writebuffer.Set((XBYTE*)&entry->size, sizeof(entry->size)))				{ delete entry; return false; }
		}

	return true;
}



/*-------------------------------------------------------------------
//	GRPFILEGGF::WriteGGFOBJECT
*/	
/**	
//	
//	writes an object form it's definition
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			13/11/2015 17:41:15
//	
//	@return 			bool : 
//
//  @param				ggfobject : 
//  @param				ggfobjectdefinition : 
//  @param				position : 
*/
/*-----------------------------------------------------------------*/
bool GRPFILEGGF::WriteGGFOBJECT(GGFOBJECT& ggfobject, GGFOBJECTDEFINITION& ggfobjectdefinition, XDWORD indexdef, XBUFFER & writebuffer)
{
	WriteGGFSTRING(ggfobject.id, writebuffer);

	// write the definition index?
	writebuffer.Set((XBYTE*)&indexdef, sizeof(indexdef));

	// write the number tag
	writebuffer.Set((XBYTE*)&ggfobject.tag, info.ptrsize);

	//XDWORD properties = ggfobject.values.GetSize();
	XDWORD properties = ggfobject.indexedvalues.GetSize();
	// write the number of properties
	writebuffer.Set((XBYTE*)&properties, sizeof(properties));

	// for every entry on the object, use iterator from indexed values
	XBINARYMAP<XDWORD, GGFVALUE*>::XITERATOR it;
	XDWORD i = 0;
	//for(XDWORD i = 0; i < properties; i++)
	for(it = ggfobject.indexedvalues.Begin(); it != ggfobject.indexedvalues.End(); it++)
		{
			i++;
			// check write size from definition
			GGFDEFINITIONENTRY* definition = NULL;
			XDWORD definitions = ggfobjectdefinition.GetSize();
			XDWORD index = 0;

			//for(XDWORD j = 0; j < definitions; j++)
			//	{
			//		// we have the entries indexed, should be ablt of finding them directly
			//		if(ggfobjectdefinition.FastGetEntry(j)->id == *ggfobject.values.GetKey(i))
			//			{
			//				definition = ggfobjectdefinition.FastGetEntry(j);
			//				index = j;
			//				// if we found it, no sense in continuing the loop
			//				if(definition)
			//					{
			//						break;
			//					}
			//			}
			//	}

			definition = ggfobjectdefinition.GetEntry(it.GetKey());
			index = it.GetKey();
			// we should have the definition to use
			if(definition)
				{
					// WRITE THE PROPERTY INDEX INSTEAD OF THE NAME
					writebuffer.Set((XBYTE*)&index, sizeof(index));

					//GGFVALUE* value = ggfobject.values.GetElement(i);
					GGFVALUE* value = it.GetElement();
					if(value->size != definition->size)
						{
							XSTRING str;
							//ggfobject.values.GetKey(i)->ToXSTRING(&str);
							definition->id.ToXSTRING(&str);
							XDEBUG_PRINTCOLOR(4,__L("Value and definition size differs object = %s, index = %d, definition = %s"), ggfobject.id.helperstring.Get(), it.GetKey(), str.Get());
							return false;
						}

					writebuffer.Set(value->buffer.Get(), definition->size);
				}
			else
				{
					// throw a warning as we are trying to write a property for whom we don't have a definition
					XSTRING str;
					//ggfobject.values.GetKey(i)->ToXSTRING(&str);
					//it.GetKey()->ToXSTRING(&str);
					XDEBUG_PRINTCOLOR(4,__L("Tried to write a property and we don't have a definition: %s"), str.Get());
					// we might want to also print the object id
				}
		}

	return true;
}




/*-------------------------------------------------------------------
//	GRPFILEGGF::GetDefinitionsSize
*/	
/**	
//	
//	gets the total size of the definitions
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			16/11/2015 14:27:04
//	
//	@return 			XDWORD : 
//
*/
/*-----------------------------------------------------------------*/
XDWORD GRPFILEGGF::GetDefinitionsSize()
{
	XDWORD size = 0;

	// number of definitions
	size += sizeof(XDWORD);

	for(XDWORD i = 0; i < definitions.GetSize(); i++)
		{
			// size of each individual definition
			size += definitions.FastGet(i)->Size();
		}

	return size;
}




/*-------------------------------------------------------------------
//	GRPFILEGGF::GetObjectsSize
*/	
/**	
//	
//	gets the complete size of all the objects
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			19/11/2015 16:45:13
//	
//	@return 			XDWORD : 
//
*/
/*-----------------------------------------------------------------*/
XDWORD GRPFILEGGF::GetObjectsSize()
{
	XDWORD size = 0;

	// number of objects
	size += sizeof(XDWORD);

	// each individual object
	for(XDWORD i = 0; i < objects.GetSize(); i++)
		{
			size += objects.FastGet(i)->Size();
		}

	return size;
}




/*-------------------------------------------------------------------
//	GRPFILEGGF::GetDataSize
*/	
/**	
//	
//	gets the size of the data area
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			20/11/2015 13:10:29
//	
//	@return 			XDWORD : 
//
*/
/*-----------------------------------------------------------------*/
XDWORD GRPFILEGGF::GetDataSize()
{
	XDWORD size = 0;

	size += sizeof(XDWORD);

	for(XDWORD i = 0; i < data.GetSize(); i++)
		{
			size += data.FastGet(i)->Size();
		}

	return size;
}

