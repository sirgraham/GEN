/*------------------------------------------------------------------------------------------
// DIOSSHReverse.HPP
*/
/**
// \class
//
//	DIO SSH reverse connexions (Linux)
//
//	@author: No se ha podido abrir el fichero de author
//
//	Date of Creation : 06/12/2017 10:35:30 
//	Last Modification : 
*/
/*	GEN  Copyright (C).  All right reserved.
/*------------------------------------------------------------------------------------------*/


#ifndef _DIOSSHREVERSE_HPP
#define _DIOSSHREVERSE_HPP

/*---- INCLUDES --------------------------------------------------------------------------*/

#ifdef LINUX
	#include <sys/types.h>
	#include <sys/stat.h>
	#include <sys/wait.h>
	#include <sys/ioctl.h>
	#include <sys/select.h>
	#include <sys/prctl.h>

	#include <unistd.h>
	#include <fcntl.h>
	#include <signal.h>
	#include <termios.h>

	#include <stdio.h>
	#include <stdlib.h>
	#include <errno.h>
	#include <string.h>
#endif

#include "XFactory.h"
#include "XString.h"
#include "XSystem.h"
#include "XFile.h"
#include "XFileINI.h"
#include "XSleep.h"
#include "XDebug.h"

#include "DIOFactory.h"
#include "DIOURL.h"
#include "DIOWebClient.h"


/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/

#define DIOSSHREVERSE_DEFAULTAPPLICATION			__L("sshrem")
#define DIOSSHREVERSE_CFGREMOTENAMEFILE				__L("sshrem.ini")
#define DIOSSHREVERSE_DEFAULTPORTSSH					22


enum DIOSSHREVERSE_RETURN_CODES 
{
	RETURN_NOERROR,
  RETURN_INVALID_ARGUMENTS,
  RETURN_CONFLICTING_ARGUMENTS,
  RETURN_RUNTIME_ERROR,
  RETURN_PARSE_ERRROR,
  RETURN_INCORRECT_PASSWORD,
  RETURN_HOST_KEY_UNKNOWN,
  RETURN_HOST_KEY_CHANGED,
};


typedef struct 
{
	enum 	{ PWT_STDIN, 
					PWT_FILE, 
					PWT_FD, 
					PWT_PASS 

				} pwtype;

  union	{ const char*  filename;
				  int					fd;
				  const char*	password;

    		} pwsrc;

  const char*  pwprompt;
  int					 verbose;
    
} DIOSSHREVERSE_ARGS;



/*---- CLASS -----------------------------------------------------------------------------*/

class DIOSSHREVERSE
{
	public:																

		static bool											GetIsInstanced		  () 
													  				{ 
																			return instance!=NULL; 
																		}

		static DIOSSHREVERSE&						GetInstance					()
																		{
																			if(!instance) instance = new DIOSSHREVERSE();
																						
																			return (*instance);	
																		}						

		static bool											DelInstance					()
																		{
																			if(instance)
																				{																			
																					delete instance;
																					instance = NULL;

																					return true;
																				} 
																				
																			return false;
																		}						


		XSTRING*											GetURLTarget					()																								{ return &URLtarget;																						}
		XSTRING*											GetLocalIP						()																								{ return &localIP;																							}
		XSTRING*											GetLogin							()																								{	return &login;																								}
		XSTRING*											GetPassword						()																								{ return &password;																							}
		
		XDWORD												GetPort								()																								{ return port;																									}
		void													SetPort								(XDWORD port)																			{ this->port = port; 																						}

		bool													DownloadCFG           (XCHAR* URL, XSTRING& publicIP, XSTRING& localIP)
																	{
																		if(!URL)   return false;

																		XBUFFER xbuffer;
																		DIOURL  _URL;
																		bool	  status  = false;
																		bool    result  = false;

																		DIOWEBCLIENT* webclient = new DIOWEBCLIENT();
																		if(!webclient)  return false;	
	
																		_URL.Set(URL);	
																		_URL.Slash_Add();			
																		_URL.Add(DIOSSHREVERSE_CFGREMOTENAMEFILE);
					
																		status = webclient->Get(_URL, xbuffer, NULL, 3);
																		if(status)
																			{
																				XFILEINI* fileini = new XFILEINI();
																				if(fileini)
																					{
																						status = fileini->AddBufferLines(XFILETXTFORMATCHAR_ASCII, xbuffer);
																						if(status)
																							{
																								status = fileini->ConvertFromLines();
																								if(status)
																									{	
																										int c = 1;
																										while(1)
																											{
																												XSTRING section;
																												XSTRING value;

																												section.Format(__L("ENTRY%02d"), c);

																												if(!fileini->SelectSection(section)) break;

																												status = fileini->ReadValue(__L("isactive"), value);																											
																												if(status)
																													{
																														if(!value.Compare(__L("yes"), true))
																															{
																																DIOURL* urlorigin = diofactory->CreateURL();
																																if(urlorigin)
																																	{
																																		XSTRING _publicIP;
																																		XSTRING _localremoteIP;

																																		status = fileini->ReadValue(__L("urlorigin"), (*urlorigin));																											
																																		if(status)
																																			{
																																				bool validaccess = false;

																																				urlorigin->ResolveURL(_publicIP);

																																				if(!publicIP.Compare(_publicIP))
																																					{
																																						validaccess = true;

																																						status = fileini->ReadValue(__L("localremoteip"), _localremoteIP);																											
																																						if(status)  
																																							{
																																								if(_localremoteIP.GetSize())
																																									{
																																										if(_localremoteIP.Compare(localIP)) validaccess = false;
																																									}
																																							}
																																					}


																																				if(validaccess)
																																					{
																																						localIP		= __L("localhost");

																																						status = fileini->ReadValue(__L("urltarget"), value);																											
																																						if(status) GetURLTarget()->Set(value);

																																						status = fileini->ReadValue(__L("localip"), value);																											
																																						if(status)  GetLocalIP()->Set(value);

																																						status = fileini->ReadValue(__L("login"), value);																											
																																						if(status) GetLogin()->Set(value);

																																						status = fileini->ReadValue(__L("password"), value);																											
																																						if(status) GetPassword()->Set(value);

																																						status = fileini->ReadValue(__L("port"), value);																											
																																						if(status) SetPort(value.ConvertToInt());		

																																						result = true;
																																					}
																																			}

																																		diofactory->DeleteURL(urlorigin);
																																	}
																												 			}
																													}

																												c++;
																											}
																									}
																							}

																						delete fileini;
																					}
																			}	

																		delete webclient;

																		return result;
																	}

		bool													DownloadCFG           (XSTRING& URL, XSTRING& publicIP, XSTRING& localIP)								
																	{ 
																		return DownloadCFG(URL.Get(), publicIP, localIP);										 
																	}

		bool													DownloadCFG           (DIOURL&  URL, XSTRING& publicIP, XSTRING& localIP)								
																	{ 
																		return DownloadCFG(URL.Get(), publicIP, localIP);										 
																	}
																
		bool													Activate							()
																	{
																		DeActivate();

																		#ifdef LINUX 

			                              XSTRING string;
																		char*		argv[] = { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
																											 NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL
																										 };																																					
																		int			argc = 0;	

																		XDEBUG_PRINTCOLOR(2, __L("-p %s ssh -o UserKnownHostsFile=/dev/null -o StrictHostKeyChecking=no -N -R %d:%s:%d %s@%s"), password.Get(), port, localIP.Get() ,DIOSSHREVERSE_DEFAULTPORTSSH, login.Get(), URLtarget.Get());

																		string = __L("app");														string.CreateOEM(argv[argc]);  argc++;																		
                                    
																		string = __L("-p");															string.CreateOEM(argv[argc]);  argc++;																		
																		string.Format(__L("%s"), password.Get());       string.CreateOEM(argv[argc]);  argc++;																		

																		string = __L("ssh");														string.CreateOEM(argv[argc]);  argc++;																		

																		string = __L("-q");															string.CreateOEM(argv[argc]);  argc++;																		

																		string = __L("-o");															string.CreateOEM(argv[argc]);  argc++;																		
																		string = __L("UserKnownHostsFile=/dev/null");   string.CreateOEM(argv[argc]);  argc++;																		
																		
																		string = __L("-o");															string.CreateOEM(argv[argc]);  argc++;																		
																		string = __L("StrictHostKeyChecking=no");			  string.CreateOEM(argv[argc]);  argc++;																		

																		string = __L("-N");															string.CreateOEM(argv[argc]);  argc++;																		
																		string = __L("-R");															string.CreateOEM(argv[argc]);  argc++;																		

																		string.Format(__L("%d:%s:%d"), port, localIP.Get(), DIOSSHREVERSE_DEFAULTPORTSSH);	 string.CreateOEM(argv[argc]);  argc++;																		
																		string.Format(__L("%s@%s"), login.Get(), URLtarget.Get());													 string.CreateOEM(argv[argc]);  argc++;																		
																																			
																		int status = main(argc, argv);																																																			
																		
																		for(int c=0; c<(sizeof(argv)/sizeof(char*)); c++)
																			{
																				delete [] argv[c];
																		  }	
																															
																		return (status==1)?true:false;

																		#else 
																		return false;
																		#endif

																	}
																	
		bool													DeActivate						()
																	{																			
																		XSTRING command;
																		bool		status 		 = false;
																		int 		returncode = 0;

																	  XSYSTEM* xsystem = xfactory->CreateSystem();
																		if(xsystem) 
																			{
																				command.Format(__L("killall -9 %s"), DIOSSHREVERSE_DEFAULTAPPLICATION);
																				status = xsystem->MakeCommand(command.Get(), &returncode);

																				xfactory->DeleteSystem(xsystem);																		
																			}

																		XDEBUG_PRINTCOLOR(2, __L("SSHReverse: Deactivate exec %s, return code: %d"), status?__L("ok"):__L("error"), returncode);

																		status = true;
																		if(returncode) status = false;
																		
																		return status;
																	}

		bool													IsRunning							()	
																	{
																		XSYSTEM* xsystem = xfactory->CreateSystem();
																		if(!xsystem) return false;

																		bool status = xsystem->IsApplicationRunning(DIOSSHREVERSE_DEFAULTAPPLICATION);
																		if(status)
																			{
																				status = false;
																																		
																				XSTRING command;
																				XSTRING publicIPtarget;
																				int     returncode = 0;																				
																																								
																				DIOURL* URLpublic = diofactory->CreateURL();
																				if(URLpublic)
																					{																				
																						URLpublic->Set(URLtarget.Get());
																						URLpublic->ResolveURL(publicIPtarget);

																						diofactory->DeleteURL(URLpublic);																						
																					}

																				if(!publicIPtarget.IsEmpty())
																					{
																						XPATH		xpath;					
																						XPATHSMANAGER::GetInstance().GetPathOfSection(XPATHSMANAGERSECTIONTYPE_ROOT, xpath);
																						xpath.Slash_Add();
																						xpath.Add(__L("backscreen"));

																						command.Format(__L("netstat -napt > %s"), xpath.Get());
																						if(xsystem->MakeCommand(command.Get(), &returncode))
																							{
																								XFILETXT* xfiletxt = new XFILETXT();
																								if(xfiletxt)
																									{	
																										if(xfiletxt->Open(xpath, true))
																											{
																												if(xfiletxt->ReadAllFile())
																													{
																														for(int c=0; c<xfiletxt->GetNLines(); c++)
																															{
																																if(xfiletxt->GetLine(c))
																																	{				
																																		XSTRING* line = xfiletxt->GetLine(c);
																																		if(line)
																																			{
																																				if((line->Find(publicIPtarget.Get() , false, 0) != XSTRING_NOTFOUND) &&
																																					 (line->Find(__L("ESTABLISHED")   , false, 0) != XSTRING_NOTFOUND) &&
																																					 (line->Find(__L("/ssh ")				  , false, 0) != XSTRING_NOTFOUND)) 																																					
																																					{	
																																						status = true;																																																																										
																																						break;							
																																					}
																																			}
																																	}																														
																															}
																													}

																												xfiletxt->Close();																												
																											}
																										
																										xfiletxt->GetPrimaryFile()->Erase(xpath);	
																										
																										delete xfiletxt;
																									}																							
																							}
																					}
																			}			

																		xfactory->DeleteSystem(xsystem);

																		XDEBUG_PRINTCOLOR(2, __L("SSHReverse: Is active? %s"), status?__L("yes"):__L("no"));
																		
																		return status;
																	}


		bool													Exec						      (XCHAR* URL, XSTRING& publicIP, XSTRING& localIP)
																	{
																		bool status = false;
																		
																		if(IsRunning())	
																			{
																				return status;
																			}
																																					
																		if(DownloadCFG(URL, publicIP, localIP))  
																			{
																				Activate();																																							
																			}
																		
																		DelInstance(); 

																		return status;	
																	}
		
		bool													Exec							    (XSTRING& URL, XSTRING& publicIP, XSTRING& localIP)								
																	{ 
																		return Exec(URL.Get(), publicIP, localIP);										 
																	}

		bool													Exec						      (DIOURL&  URL, XSTRING& publicIP, XSTRING& localIP)								
																	{ 
																		return Exec(URL.Get(), publicIP, localIP);										 
																	}
																
	

	private:
																	DIOSSHREVERSE					()
																	{
																		Clean();
																	}

																	DIOSSHREVERSE					(DIOSSHREVERSE const&);			// Don't implement  

		virtual							     	   ~DIOSSHREVERSE					()
																	{
																		Clean();
																	}

		void													operator =						(DIOSSHREVERSE const&);			// Don't implement	

		void													Clean									()
																	{
																		URLtarget.Empty();
																		localIP.Empty();		
																		login.Empty();
																		password.Empty();														

																		port			= 0;
																	}

		#ifdef LINUX
		int														posix_openpt					(int flags)
																	{
																	  return open("/dev/ptmx", flags);
																	}

		int														parse_options					(int argc, char *argv[] )
																	{
																		int error	=	-1;
																		int opt;

																		// Set the default password source to stdin
																		args.pwtype			= DIOSSHREVERSE_ARGS::PWT_STDIN;
																		args.pwsrc.fd		= 0;

																		#define VIRGIN_PWTYPE		if( args.pwtype!=DIOSSHREVERSE_ARGS::PWT_STDIN ) { \
																														error = RETURN_CONFLICTING_ARGUMENTS; }

																		while( (opt = getopt(argc, argv, "+f:d:p:P:heVv"))!=-1 && error==-1 ) 
  																		{
																				switch( opt ) 
																					{																							    							
																						case 'p': // Password is given on the command line
	    																								VIRGIN_PWTYPE;

	    																								args.pwtype					=	DIOSSHREVERSE_ARGS::PWT_PASS;
																											args.pwsrc.password	=	strdup(optarg);
            
																											// Hide the original password from the command line
																											{	int i;

                																				for( i=0; optarg[i]!='\0'; ++i )
                    																				optarg[i]='z';
            																					}
	    																								break;
	    					        																		
																					}
																			}

																		if( error>=0 )
   																		{    		
																				return -(error+1);
																			}
																		 else 
																			{
  																			return optind;
  																		}
																	}


		int														runprogram						(int argc, char *argv[])
																	{
																		struct winsize ttysize; // The size of our tty

																		// We need to interrupt a select with a SIGCHLD. In order to do so, we need a SIGCHLD handler
																		signal( SIGCHLD,sigchld_handler );

																		// Create a pseudo terminal for our process
																		masterpt=posix_openpt(O_RDWR);

																		if(masterpt==-1) 
  																		{
																				//perror("Failed to get a pseudo terminal");
																				return RETURN_RUNTIME_ERROR;
																			}

																		fcntl(masterpt, F_SETFL, O_NONBLOCK);

																		if(grantpt( masterpt ) != 0) 
  																		{
																				//perror("Failed to change pseudo terminal's permission");
																				return RETURN_RUNTIME_ERROR;
																			}
    
   
																		if(unlockpt( masterpt ) != 0) 
  																		{
																				//perror("Failed to unlock pseudo terminal");
																				return RETURN_RUNTIME_ERROR;
																			}

																		ourtty=open("/dev/tty", 0);
																		if( ourtty!=-1 && ioctl( ourtty, TIOCGWINSZ, &ttysize )==0 ) 
  																		{
    																		signal(SIGWINCH, window_resize_handler);
																				ioctl( masterpt, TIOCSWINSZ, &ttysize );
																			}

																		const char *name = (const char*)ptsname(masterpt);
																		int slavept;
  

																		int childpid=fork();
																		if( childpid==0 ) 
  																		{
																				// Child

																				// Detach us from the current TTY
																				setsid();
																					// This line makes the ptty our controlling tty. We do not otherwise need it open
																				slavept=open(name, O_RDWR );
																				close( slavept );
	
																				close( masterpt );

																				char **new_argv= (char**)malloc(sizeof(char*) * (argc+1));

																				int i;

																				for( i=0; i<argc; ++i ) 
																					{
	    																			new_argv[i] = argv[i];
																					}

																				new_argv[i] = NULL;

																				
																			  //XSTRING name;																										
																				//name.Format(__L("%s-1"), DIOSSHREVERSE_DEFAULTAPPLICATION);
																				//name.CreateOEM(new_argv[0]);
																																								
																				execvp("ssh", new_argv );

																				//perror("sshpass: Failed to run command");

																				exit(RETURN_RUNTIME_ERROR);
																			} 
																		 else 
   																		{
   																			if( childpid<0 ) 
   																				{
																						//perror("sshpass: Failed to create child process");
																						return RETURN_RUNTIME_ERROR;
    																			}
																			}
	
																		// We are the parent
																		slavept=open(name, O_RDWR|O_NOCTTY );

																		int status=0;
																		int terminate=0;
																		pid_t wait_id;
  
																		sigset_t sigmask, sigmask_select;

																		// Set the signal mask during the select
																		sigemptyset(&sigmask_select);

																		// And during the regular run
																		sigemptyset(&sigmask);
																		sigaddset(&sigmask, SIGCHLD);

																		sigprocmask( SIG_SETMASK, &sigmask, NULL );

																		do {
																					if( !terminate ) 
																						{
	    																				fd_set readfd;

	    																				FD_ZERO(&readfd);
	    																				FD_SET(masterpt, &readfd);

	    																				int selret=pselect( masterpt+1, &readfd, NULL, NULL, NULL, &sigmask_select );

	    																				if( selret>0 ) 
	    																					{
																									if( FD_ISSET( masterpt, &readfd ) ) 
																										{
																											int ret;
		    																							if( (ret=handleoutput( masterpt )) ) 
		    																								{
																													// Authentication failed or any other error

																													// handleoutput returns positive error number in case of some error, and a negative value
																													// if all that happened is that the slave end of the pt is closed.
																													if( ret>0 ) 
                        																		{
																															close( masterpt ); // Signal ssh that it's controlling TTY is now closed
																															close(slavept);
                       																		 }

																													terminate=ret;

																													if( terminate ) 
                        																		{
																															close( slavept );																																																														
                       																			}
		    																								}
																										}
	    																					}
	    																											
	    																				wait_id = waitpid( childpid, &status, WNOHANG );							    			
																						} 
																					 else 
				 																		{																							
	    																				wait_id = waitpid( childpid, &status, 0 );																								
																						}
					
																			} while( wait_id==0 || (!WIFEXITED( status ) && !WIFSIGNALED( status )) );
																	 

																		if( terminate>0 ) 
																			{
																				return terminate;
																			}
																		 else 
   																		{
   																			if( WIFEXITED( status ) ) 
   		  																			 return WEXITSTATUS(status);
    																			else return 255;
   																		}
																	}


		int														handleoutput( int fd )
																	{
																		// We are looking for the string
																		static int prevmatch=0; // If the "password" prompt is repeated, we have the wrong password.
																		static int state1, state2;
																		static int firsttime = 1;
																		static const char *compare1		= "assword"; // Asking for a password
																		static const char compare2[]	= "The authenticity of host "; // Asks to authenticate host
 
																		// static const char compare3[]="WARNING: REMOTE HOST IDENTIFICATION HAS CHANGED!"; // Warns about man in the middle attack
																		// The remote identification changed error is sent to stderr, not the tty, so we do not handle it.
																		// This is not a problem, as ssh exists immediately in such a case
																		char buffer[256];
																		int ret=0;

																		if( args.pwprompt ) 
  																		{
																				compare1 = args.pwprompt;
																			}

																		if( args.verbose && firsttime ) 
  																		{
																				firsttime=0;
																				//fprintf(stderr, "SSHPASS searching for password prompt using match \"%s\"\n", compare1);
																			}

																		int numread=read(fd, buffer, sizeof(buffer)-1 );
																		buffer[numread] = '\0';
  
																		if( args.verbose ) 
  																		{
    																		//fprintf(stderr, "SSHPASS read: %s\n", buffer);
																			}

																		state1=match( compare1, buffer, numread, state1 );

																			// Are we at a password prompt?
																		if( compare1[state1]=='\0' ) 
  																		{
																				if( !prevmatch ) 
																					{
																						//if( args.verbose ) fprintf(stderr, "SSHPASS detected prompt. Sending password.\n");
	    																			write_pass( fd );
	    																			state1=0;
	    																			prevmatch=1;
																					} 
																				 else 
			 																		{
	    																			// Wrong password - terminate with proper error code
         																		//if( args.verbose ) fprintf(stderr, "SSHPASS detected prompt, again. Wrong password. Terminating.\n");
	    																			ret=RETURN_INCORRECT_PASSWORD;
																					}
																			}

																		if( ret==0 ) 
  																		{
    																		state2=match( compare2, buffer, numread, state2 );

																				// Are we being prompted to authenticate the host?
																				if( compare2[state2]=='\0' ) 
      																		{
																						//if( args.verbose ) fprintf(stderr, "SSHPASS detected host authentication prompt. Exiting.\n");
																						ret=RETURN_HOST_KEY_UNKNOWN;
																					}
																			}

																		return ret;
																	}


		int														match( const char *reference, const char *buffer, ssize_t bufsize, int state )
																	{
																		// This is a highly simplisic implementation. It's good enough for matching "Password: ", though.
																		int i;
																		for( i=0;reference[state]!='\0' && i<bufsize; ++i ) 
  																		{
																				if( reference[state]==buffer[i] )
	   																			{
	  																				state++;
	   																			}
																				 else 
																					{
	    																			state=0;
	    																			if( reference[state]==buffer[i] )
																						state++;
																					}
																			}

																		return state;
																	}


			void												write_pass						(int fd)
																	{
																		switch( args.pwtype ) 
  																		{
																				case DIOSSHREVERSE_ARGS::PWT_STDIN	:	write_pass_fd(STDIN_FILENO, fd );
																																							break;
											
																				case DIOSSHREVERSE_ARGS::PWT_FD			:	write_pass_fd( args.pwsrc.fd, fd );
																																							break;
											
																				case DIOSSHREVERSE_ARGS::PWT_FILE		: {	int srcfd=open( args.pwsrc.filename, O_RDONLY );
	    																																					if(srcfd != -1) 
	    																																						{
																																										write_pass_fd( srcfd, fd );
																																										close( srcfd );
		    																																					}
																																							}
																																							break;
											
																				case DIOSSHREVERSE_ARGS::PWT_PASS		:	write( fd, args.pwsrc.password, strlen( args.pwsrc.password ) );
																																							write( fd, "\n", 1 );
																																							break;
																			}
																	}


		void													write_pass_fd					(int srcfd, int dstfd)
																	{
																		int done=0;

																		while(!done) 
  																		{
																				char buffer[40];
																				int i;
																				int numread=read( srcfd, buffer, sizeof(buffer) );
		
																				done=(numread<1);
																				for( i=0; i<numread && !done; ++i ) 
																					{
	    																			if( buffer[i]!='\n' )
					 																				 write( dstfd, buffer+i, 1 );
	    																				else done=1;
																					}
																			}
    
																		write( dstfd, "\n", 1 );
																	}



		static void										window_resize_handler	(int signum)
																	{
																		struct winsize ttysize; // The size of our tty

																		if(ioctl( ourtty, TIOCGWINSZ, &ttysize )==0)  ioctl(masterpt, TIOCSWINSZ, &ttysize);
																	}



		static void										sigchld_handler				(int signum)
																	{
	
																	}


		int														main									(int argc, char *argv[])
																	{
																		int status		 = -1;
																		int opt_offset = parse_options( argc, argv );

																		if( opt_offset < 0 ) 
  																		{																		
																				return status;
																			}

																		if( argc - opt_offset < 1 ) 
  																		{																				
																				return status;
																			}

																		pid_t pID = fork();

																		switch(pID)
																			{
																				case -1	:	break;

																				case  0	: { XSTRING name;
																										
																										name = 	DIOSSHREVERSE_DEFAULTAPPLICATION;
																										XSTRING_CREATEOEM(name, _namechar)
																										prctl(PR_SET_NAME, _namechar, NULL, NULL, NULL); // name: myProcess
																										XSTRING_DELETEOEM(_namechar)

																										runprogram(argc - opt_offset, argv + opt_offset );	

																										exit(RETURN_RUNTIME_ERROR);
																									}
																									break;

																				default	:	{ 
																										status = 0;

																										XTIMER* xtimer = xfactory->CreateTimer();
																										if(xtimer)
																											{
																												xtimer->Reset();

																												do{	int wait_id = waitpid(pID, &status, WNOHANG);	

																														if(xtimer->GetMeasureSeconds() > 5) 
																															{
																																status = 1;
																																break;
																															}

																													} while(WIFEXITED(status));

																												xfactory->DeleteTimer(xtimer);
																											}
																									}
																									break;
																			}

																		return status; 																																			
																	}
		#endif

		XSTRING												URLtarget;
		XSTRING												localIP;		
		XSTRING												login;
		XSTRING												password;
		XDWORD												port;	
		
		static DIOSSHREVERSE*					instance;	

		#ifdef LINUX

		DIOSSHREVERSE_ARGS						args;
		static int										ourtty; // Our own tty
		static int										masterpt;
		#endif		
};

/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/

DIOSSHREVERSE*	 DIOSSHREVERSE::instance = NULL;

#ifdef LINUX
int							 DIOSSHREVERSE::ourtty		= 0; 
int							 DIOSSHREVERSE::masterpt  = 0;             
#endif


#endif