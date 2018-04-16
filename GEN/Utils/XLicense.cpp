
/*------------------------------------------------------------------------------------------
//	XLICENSE.CPP
//	
//	Licence Generator class
//   
//	Author						: Abraham J. Velez
//	Date Of Creation	: 15/05/2014 16:14:23
//	Last Modification	:	
//	
//	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/

#include "XFactory.h"
#include "XPublisher.h"
#include "XFileHash.h"
#include "XFile.h"
#include "XFileINI.h"

#include "HashSHA1.h"
#include "CipherAES.h"

#include "DIOFactory.h"
#include "DIOMAC.h"
#include "DIOWebClient.h"
#include "DIOStreamDeviceIP.h"
#include "DIOStreamIPLocalEnumDevices.h"

#include "XLicense.h"

#include "XMemory.h"
	
/*---- GENERAL VARIABLE ------------------------------------------------------------------*/
	
	
/*---- CLASS MEMBERS ---------------------------------------------------------------------*/




/*-------------------------------------------------------------------
//	XLICENSE::XLICENSE
*/	
/**	
//	
//	Class Constructor XLICENSE
//	
//	@author				Abraham J. Velez
//	@version			15/05/2014 16:51:54
//	
 
 
//  @param				publisher : 
*/
/*-----------------------------------------------------------------*/
XLICENSE::XLICENSE()
{
	Clean();

	license = new XBUFFER();
	
	RegisterEvent(XLICENSEXEVENTTYPE_INVALID);
	RegisterEvent(XLICENSEXEVENTTYPE_EXPIRED);
}



/*-------------------------------------------------------------------
//	XLICENSE::~XLICENSE
*/	
/**	
//	
//	 Class Destructor XLICENSE
//	
//	@author				Abraham J. Velez
//	@version			15/05/2014 16:21:22
//	
*/
/*-----------------------------------------------------------------*/
XLICENSE::~XLICENSE()
{
	DeRegisterEvent(XLICENSEXEVENTTYPE_INVALID);
	DeRegisterEvent(XLICENSEXEVENTTYPE_EXPIRED);

	delete license;

	Clean();
}




/*-------------------------------------------------------------------
//	XLICENSE::GenerateMachineID
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			15/05/2014 16:42:58
//	
//	@return 			bool : 
//
//  @param				licenseID : 
*/
/*-----------------------------------------------------------------*/
bool XLICENSE::GenerateMachineID(XLICENSEID& licenseID)
{
	DIOSTREAMIPLOCALENUMDEVICES* enumdevices = (DIOSTREAMIPLOCALENUMDEVICES*)diofactory->CreateStreamEnumDevices(DIOSTREAMENUMTYPE_IP_LOCAL);
	if(enumdevices)
		{
			enumdevices->Search();

			XQWORD	basicID			= 0;
			XSTRING	basicstring;
			XBUFFER basicxbuffer;
					
			for(int c=0;c<(int)enumdevices->GetDevices()->GetSize();c++)
				{				
					DIOSTREAMDEVICEIP* device = (DIOSTREAMDEVICEIP*)enumdevices->GetDevices()->Get(c);
					if(device) 
						{
							DIOMAC* mac = device->GetMAC();
							if(mac)
								{
									XQWORD macID = 0;

									for(int d=0; d<DIOMAC_MAXSIZE; d++)
										{
											macID |= mac->Get()[d];
											macID<<=8;
										}

									basicID ^= macID; 
								}
						}
				}

			diofactory->DeleteStreamEnumDevices(enumdevices);				

			basicstring.Format(__L("%02X%02X%llX%llX%02X%02X"), XLICENSE_VERSION, XLICENSE_SUBVERSION, basicID, ~basicID, XLICENSE_VERSION, XLICENSE_SUBVERSION);

			HASHSHA1* sha1 = new HASHSHA1();	
			if(sha1)
				{
					basicstring.ConvertHexStringToBuffer(basicxbuffer);
					sha1->Do(basicxbuffer);

					XDWORD index			= 0;
					XDWORD partID			= 0;
					int		 sizeofpart = sizeof(XDWORD)/sizeof(XBYTE);

					for(int c=0; c<XLICENSE_MAXIDPARTS; c++)
						{			
							partID = 0;
							for(int d=0; d<sizeofpart; d++)
								{	
									partID |= (XBYTE)sha1->GetResult()->GetByte(index++);
									if(d!=(sizeofpart-1)) partID<<=8;
									if(index>=sha1->GetResult()->GetSize()) index = 0;
								}

							licenseID.SetPart(c, partID);
						}

					delete sha1;

					return true;
				}
		}

	return false;
}




/*-------------------------------------------------------------------
//	XLICENSE::Generate
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			20/05/2014 9:49:35
//	
//	@return 			bool : 
//
//  @param				licenseID : 
//  @param				applicationID : 
//  @param				license : 
*/
/*-----------------------------------------------------------------*/
bool XLICENSE::Generate(XLICENSEID& licenseID, XSTRING* applicationID, XBUFFER* license)
{	
	CIPHERAES							cipherAES;
	CIPHERKEYSYMMETRICAL  key;		
	XBUFFER								xbufferkey;
	XBUFFER								xbuffercipher;
	XBUFFER*							_license			 = license;
	XSTRING*							_applicationID = applicationID;

	if(!_license) _license = this->license;
	if(!_license) return false;

	if(!_applicationID) _applicationID = &this->applicationID;
	if(!_applicationID) return false;
		
	_license->Delete();		
	licenseID.GetXBuffer(xbuffercipher);

	GetBufferKeyFromMachineID((*_applicationID), xbufferkey);
	
	key.Set(xbufferkey);

	cipherAES.SetKey(&key);
	cipherAES.SetChainingMode(CIPHERCHAININGMODE_CBC);				
	cipherAES.SetPaddingType(XBUFFER_PADDINGTYPE_NONE);

	if(cipherAES.Cipher(xbuffercipher))
		{
			_license->Delete();
			_license->Add(cipherAES.GetResult());

			return true;
		}

	return false;
}



/*-------------------------------------------------------------------
//	XLICENSE::GetBufferKeyFromMachineID
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			20/05/2014 9:19:02
//	
//	@return 			bool : 
//
//  @param				xbufferkey : 
*/
/*-----------------------------------------------------------------*/
bool XLICENSE::GetBufferKeyFromMachineID(XSTRING& applicationID, XBUFFER& xbufferkey)
{
	HASHSHA1 sha1;	
	
	xbufferkey.Delete();

	XSTRING_CREATEOEM(applicationID, keychar)
	xbufferkey.Add((XBYTE*)keychar, applicationID.GetSize());
	XSTRING_DELETEOEM(keychar)	

	sha1.Do(xbufferkey);
	
	xbufferkey.Delete();
	xbufferkey.Add(sha1.GetResult());

	xbufferkey.Resize(16);

	return xbufferkey.GetSize()?true:false;
}




/*-------------------------------------------------------------------
//	XLICENSE::CipherExpirationDate
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			20/05/2014 13:12:14
//	
//	@return 			bool : 
//
//  @param				cipher : 
//  @param				applicationID : 
//  @param				expirationdate : 
*/
/*-----------------------------------------------------------------*/
bool XLICENSE::CipherExpirationDate(bool cipher, XSTRING& applicationID, XSTRING& expirationdate)
{
	XBUFFER								xbufferkey;
	CIPHERAES							cipherAES;
	CIPHERKEYSYMMETRICAL  key;		
	bool									status = false;

	if(!GetBufferKeyFromMachineID(applicationID, xbufferkey)) return false;

	key.Set(xbufferkey);

	cipherAES.SetKey(&key);
	cipherAES.SetChainingMode(CIPHERCHAININGMODE_CBC);				
	cipherAES.SetPaddingType(XBUFFER_PADDINGTYPE_SPACES);

	if(cipher)		
		{
			XSTRING_CREATEOEM(expirationdate, expirationdatechar)
			status = cipherAES.Cipher((XBYTE*)expirationdatechar, expirationdate.GetSize());
			XSTRING_DELETEOEM(expirationdatechar)	

			if(status)			
				{						
					cipherAES.GetResult()->ConvertToBase64(expirationdate);
					return true;
				}
		}
	 else
		{
			XBUFFER xbuffer;

			xbuffer.ConvertFromBase64(expirationdate);

			status = cipherAES.Uncipher(xbuffer);			
			if(status)			
				{			
					expirationdate.Empty();

					for(XDWORD c=0;  c<cipherAES.GetResult()->GetSize(); c++)
						{
							expirationdate.Add((XCHAR)cipherAES.GetResult()->GetByte(c));
						}

					expirationdate.DeleteCharacter(0x20, XSTRINGCONTEXT_ATEND);

					return true;
				}
		}

	return status;
}





/*-------------------------------------------------------------------
//	XLICENSE::Get
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			16/05/2014 12:09:20
//	
//	@return 			bool : 
//
//  @param				licensestring : 
*/
/*-----------------------------------------------------------------*/
bool XLICENSE::Get(XSTRING& licensestring)
{
	licensestring.Empty();

	if(!license)						return false;
	if(!license->GetSize()) return false;

	for(XDWORD c=0; c<license->GetSize(); c++)
		{
			XSTRING hexchar;
			
			hexchar.Format(__L("%02X"), license->GetByte(c));
			licensestring += hexchar;
		}

	return true;
}






/*-------------------------------------------------------------------
//	XLICENSE::LoadFromFile
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			20/05/2014 14:34:37
//	
//	@return 			bool : 
//
//  @param				xpath : 
//  @param				applicationID : 
//  @param				license : 
//  @param				expirationdate : 
*/
/*-----------------------------------------------------------------*/
bool XLICENSE::LoadFromFile(XPATH& xpath, XSTRING& applicationID, XBUFFER* license, XSTRING* expirationdate)
{
	if(!license) return false;

	XFILE*		xfile		= NULL;
	XBUFFER 	xbuffer;
	bool			status	= false;

	xfile = xfactory->Create_File();
	if(!xfile) return false;

	if(xfile->Open(xpath)) 
		{
			xbuffer.Resize(xfile->GetSize());

			status = xfile->Read(xbuffer.Get(), xbuffer.GetSize());
	
			xfile->Close();
			xfactory->Delete_File(xfile);
			
			if(status) status = LoadFromBuffer(xbuffer, applicationID, license, expirationdate);
		}
	
	return status;
}




/*-------------------------------------------------------------------
//	XLICENSE::LoadFromURL
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			20/05/2014 14:34:14
//	
//	@return 			bool : 
//
//  @param				url : 
//  @param				timeout : 
//  @param				IPlocal : 
//  @param				applicationID : 
//  @param				license : 
//  @param				expirationdate : 
*/
/*-----------------------------------------------------------------*/
bool XLICENSE::LoadFromURL(DIOURL& url, int timeout, XSTRING* IPlocal, XSTRING& applicationID, XBUFFER* license, XSTRING* expirationdate)
{	
	DIOWEBCLIENT* webclient = new DIOWEBCLIENT;	
	if(!webclient) return false;

	XBUFFER xbuffer;
	bool		status;

	status = webclient->Get(url.Get(), xbuffer, NULL, timeout, IPlocal);
	if(status) status = LoadFromBuffer(xbuffer, applicationID, license, expirationdate);
	
	delete webclient;

	return status;
}



/*-------------------------------------------------------------------
//	XLICENSE::LoadFromBuffer
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			20/05/2014 14:31:39
//	
//	@return 			bool : 
//
//  @param				xbuffer : 
//  @param				applicationID : 
//  @param				license : 
//  @param				expirationdate : 
*/
/*-----------------------------------------------------------------*/
bool XLICENSE::LoadFromBuffer(XBUFFER& xbuffer, XSTRING& applicationID, XBUFFER* license, XSTRING* expirationdate)
{
	if(!license) return false;

	XFILEINI		fileini;
	XLICENSEID	licenseID;
	XSTRING			licenseIDstring;
	XSTRING			licenseIDstring2;
	XSTRING			licensestring;	
	XSTRING			licenseexprirationstring;	
	XBUFFER*		_license = license;

	if(!_license) _license = this->license;
	if(!_license) return false;

	fileini.AddBufferLines(XFILETXTFORMATCHAR_ASCII, xbuffer);

	fileini.ConvertFromLines();
	
	fileini.ReadValue(XLICENSE_FILESECTION, XLICENSE_FILEVALUE_ID					, licenseIDstring);
	fileini.ReadValue(XLICENSE_FILESECTION, XLICENSE_FILEVALUE_LICENSE		, licensestring);
	fileini.ReadValue(XLICENSE_FILESECTION, XLICENSE_FILEVALUE_EXPIRATION	, licenseexprirationstring);

	GenerateMachineID(licenseID);	
	licenseID.GetXString(licenseIDstring2);

	if(licenseIDstring.Compare(licenseIDstring2)) return false;
	
	CipherExpirationDate(false, applicationID, licenseexprirationstring);

	if(expirationdate) (*expirationdate) = licenseexprirationstring;

	licensestring.ConvertHexStringToBuffer((*_license));

	return true;
}







/*-------------------------------------------------------------------
//	XLICENSE::CheckMasterCreation
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			20/05/2014 12:56:12
//	
//	@return 			bool : 
//
//  @param				xpath : 
//  @param				licenseID : 
//  @param				applicationID : 
//  @param				expirationseconds : 
*/
/*-----------------------------------------------------------------*/
bool XLICENSE::CheckMasterCreation(XPATH& xpath, XLICENSEID& licenseID, XSTRING& applicationID, int expirationseconds)
{
	XFILEINI		fileini;
	XSTRING     licensestring;

	if(!fileini.Open(xpath)) return false;
	fileini.ReadValue(XLICENSE_FILESECTION, XLICENSE_FILEVALUE_LICENSE, licensestring);
	fileini.Close();

	if(!applicationID.Compare(licensestring)) 
		{	
			XFILEINI		fileini;
			XSTRING			licenseIDstring;
			XSTRING			licensestring;
			XSTRING			licenseexpiration;			
			XBUFFER			licencia;
			XDATETIME*	expirationdatetime;
	
			licenseID.GetXString(licenseIDstring);
			if(!Generate(licenseID, &applicationID, &licencia)) return false;

			licensestring.ConvertHexStringFromBuffer(licencia);

			expirationdatetime = xfactory->CreateDateTime();
			if(!expirationdatetime) return false;

			expirationdatetime->SetToZero();
			if(expirationseconds)
				{
					expirationdatetime->Read();
					expirationdatetime->AddSeconds((int)expirationseconds);
				}
			
			expirationdatetime->GetDateTimeToString(XDATETIME_FORMAT_STANDARD,	licenseexpiration);
			xfactory->DeleteDateTime(expirationdatetime);

			if(!CipherExpirationDate(true, applicationID, licenseexpiration)) return false;
	
			if(!fileini.Create(xpath)) return false;

			fileini.CreateSection(XLICENSE_FILESECTION);
			fileini.WriteValue(XLICENSE_FILESECTION, XLICENSE_FILEVALUE_ID				, licenseIDstring);
			fileini.WriteValue(XLICENSE_FILESECTION, XLICENSE_FILEVALUE_LICENSE		, licensestring);
			fileini.WriteValue(XLICENSE_FILESECTION, XLICENSE_FILEVALUE_EXPIRATION, licenseexpiration);

			fileini.Close();

			return true;
		}

	return false;
}



/*-------------------------------------------------------------------
//	XLICENSE::SetEvent
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			19/05/2014 13:17:51
//	
//	@return 			bool : 
//
//  @param				type : 
*/
/*-----------------------------------------------------------------*/
bool XLICENSE::SetEvent(XLICENSEXEVENTTYPE type)
{
	XLICENSEXEVENT xlicenseevent(this, type);

	xlicenseevent.Set(this);
	PostEvent(&xlicenseevent);
	
	return true;
}




