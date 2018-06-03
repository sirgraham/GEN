#---- Common --------------------------------------------------------------------------------------------------------------

FLAGS += -MMD -MP       

ifeq "$(TOOLCHAIN)" "NATIVE"
	CC = gcc
	CCPP = g++
endif

ifeq "$(USESANITIZER)" "YES"  
	FLAGS += -fsanitize=address -fno-omit-frame-pointer
endif

#---- PC --------------------------------------------------------------------------------------------------------------

ifeq "$(ARCHITECTURE)" "PC"
	CC		  = gcc
	CCPP	  = g++
	FLAGS	+= -DHW_PC
	FLAGS	+= -D__STDC_CONSTANT_MACROS
  #FLAGS  += -m64
 
	CPPDIR += /usr/include/postgresql  
	
endif

#---- PC_OLD --------------------------------------------------------------------------------------------------------------

ifeq "$(ARCHITECTURE)" "PC_OLD"
	CC		  = gcc3
	CCPP	  = g++3
	FLAGS	+= -DHW_PC
	FLAGS	+= -DHW_PC_OLD
	
	FLAGS	+= -D__STDC_CONSTANT_MACROS
	FLAGS += -w
	FLAGS += -Wfatal-errors
  
  LIBS	 += -ldl
	
 #FLAGS  += -m64
 
	CPPDIR += /usr/include/postgresql  
	
endif



#---- RASPBERRY PI ----------------------------------------------------------------------------------------------

ifeq "$(ARCHITECTURE)" "RASPBERRYPI"  

	ifeq "$(TOOLCHAIN)" "LINARO"
	CC	    = arm-linux-gnueabihf-gcc
	CCPP	= arm-linux-gnueabihf-g++
	CPPDIR += /usr/gcc-linaro-arm-linux-gnueabihf-raspbian/include
	endif

	ifeq "$(TOOLCHAIN)" "RASPBIAN"
	CC	    = arm-linux-gnueabihf-gcc
	CCPP	= arm-linux-gnueabihf-g++
	#CPPDIR += /opt/raspbian-toolchain/include
	endif

	#CC = clang
	#CCPP = clang++

	#FLAGS += -ccc-host-triple arm-linux-eabi
	#FLAGS += -mfloat-abi=hard -mfpu=vfp

#	CC	    = arm-linux-gnueabi-gcc
#	CCPP	= arm-linux-gnueabi-g++
	
	FLAGS  += -DSTANDALONE
	FLAGS  += -D__STDC_CONSTANT_MACROS
	FLAGS  += -D__STDC_LIMIT_MACROS
	FLAGS  += -DTARGET_POSIX
	FLAGS  += -D_LINUX
	FLAGS  += -D_REENTRANT
	FLAGS  += -D_LARGEFILE64_SOURCE
	FLAGS  += -D_FILE_OFFSET_BITS=64
	FLAGS  += -U_FORTIFY_SOURCE
	FLAGS  += -DHAVE_LIBBCM_HOST 
	FLAGS  += -DUSE_EXTERNAL_LIBBCM_HOST
	FLAGS  += -DHAVE_LIBOPENMAX=2
	FLAGS  += -DOMX
	FLAGS  += -DOMX_SKIP64BIT
	FLAGS  += -DUSE_EXTERNAL_OMX
	FLAGS  += -DUSE_VCHIQ_ARM
	FLAGS  += -DPIC
	FLAGS  += -fPIC
	FLAGS  += -ftree-vectorize 
	#FLAGS  += -ffast-math
	#FLAGS  += -DUSE_RESTRICT
	FLAGS  += -pipe 	
	FLAGS  += -Wno-psabi
	
	FLAGS  += -DHW_RASPBERRYPI
		
	
	ifeq "$(SYSROOT)" "JESSIE"
	FLAGS += -Wl,-rpath-link,/opt/sysroot/usr/lib/arm-linux-gnueabihf/,-rpath-link,/opt/sysroot/lib/arm-linux-gnueabihf/#/opt/sysroot/lib

	CPPDIR += /opt/sysroot/opt/vc/include/interface/vcos/pthreads
	CPPDIR += /opt/sysroot/opt/vc/include/interface/vmcs_host/linux	

	FLAGS += -isystem /opt/sysroot/usr/include/arm-linux-gnueabihf/
	FLAGS += -isystem /opt/sysroot/include/postgresql
	FLAGS += -isystem /opt/sysroot/include
	FLAGS += -isystem /opt/sysroot/usr/include

	CPPDIR += /opt/sysroot/opt/vc/include
	CPPDIR += /opt/sysroot/opt/vc/src/hello_pi/libs/ilclient/

	FLAGS += --sysroot=/opt/sysroot/

	LIBS	 += -L /opt/sysroot	
	LIBS	 += -L /opt/sysroot/lib	
	LIBS	 += -L /opt/sysroot/opt/vc/lib
	
	LIBS	 += -L /opt/sysroot/usr/lib/arm-linux-gnueabihf/
	LIBS	 += -L /opt/sysroot/lib/arm-linux-gnueabihf/
	
	LIBS	 += -L /opt/sysroot/opt/vc/src/hello_pi/libs/ilclient/

	else
		ifeq "$(SYSROOT)" "NATIVE"
		CPPDIR += /opt/vc/include
		CPPDIR += /opt/vc/src/hello_pi/libs/ilclient/

		LIBS	 += -L /opt/sysroot	
		LIBS	 += -L /opt/sysroot/lib	
		LIBS	 += -L /opt/vc/lib

		LIBS	 += -L /opt/vc/src/hello_pi/libs/ilclient/
		else
		# !! SYSROOT !!
		FLAGS += -Wl,-rpath-link,/usr/arm-linux-gnueabihf/lib

		# !! SYSROOT !!
		CPPDIR += /usr/gcc-linaro-arm-linux-gnueabihf-raspbian/include/interface/vcos/pthreads
		CPPDIR += /usr/gcc-linaro-arm-linux-gnueabihf-raspbian/include/interface/vmcs_host/linux	
		CPPDIR += /usr/gcc-linaro-arm-linux-gnueabihf-raspbian/lib/ilclient

		CPPDIR += /usr/gcc-linaro-arm-linux-gnueabihf-raspbian/include/postgresql
		CPPDIR += /usr/arm-linux-gnueabihf/include/postgresql
		CPPDIR += /usr/arm-linux-gnueabihf/include
		
		# Like this for now, need to fix it with sysroot
		# !! SYSROOT !!
		CPPDIR += /root/pifirmware/opt/vc/include

		# !! SYSROOT !!
		LIBS	 += -L /usr/arm-linux-gnueabihf	
		LIBS	 += -L /usr/arm-linux-gnueabihf/lib	
		
		# !! SYSROOT !!
		LIBS	 += -L /usr/gcc-linaro-arm-linux-gnueabihf-raspbian/lib
		LIBS	 += -L /usr/gcc-linaro-arm-linux-gnueabihf-raspbian/lib/vc
		LIBS	 += -L /usr/gcc-linaro-arm-linux-gnueabihf-raspbian/lib/ilclient
		endif
	endif
	#CPPDIR += /opt/vc/include
	#CPPDIR += /opt/vc/include/interface/vcos/pthreads
	#CPPDIR += /opt/vc/include/interface/vmcs_host/linux	
	#CPPDIR += /opt/vc/lib/ilclient
	#CPPDIR += /opt/vc/src/hello_pi/libs/ilclient
	#CDIR += /opt/vc/src/hello_pi/libs/ilclient

	# !! SYSROOT !!
	LIBS	 += -L /opt/vc/lib
	LIBS	 += -L /opt/vc/lib/vc
	LIBS	 += -L /opt/vc/lib/ilclient
	LIBS	 += -L /opt/vc/src/hello_pi/libs/ilclient

	LIBS	 += -L ../../../$(ADDDIRECTORYGEN)GEN/Platforms/Linux/MySQL
			
endif

#---- ODROID C1 ----------------------------------------------------------------------------------------------

ifeq "$(ARCHITECTURE)" "ODROIDC1"  

	CC	    = arm-linux-gnueabihf-gcc
	CCPP	  = arm-linux-gnueabihf-g++

#	CC	    = arm-linux-gnueabi-gcc
#	CCPP	  = arm-linux-gnueabi-g++
	
	FLAGS  += -DSTANDALONE
	FLAGS  += -D__STDC_CONSTANT_MACROS
	FLAGS  += -D__STDC_LIMIT_MACROS
	FLAGS  += -DTARGET_POSIX
	FLAGS  += -D_LINUX
	FLAGS  += -D_REENTRANT
	FLAGS  += -D_LARGEFILE64_SOURCE
	FLAGS  += -D_FILE_OFFSET_BITS
  FLAGS  += -U_FORTIFY_SOURCE
	#FLAGS  += -DHAVE_LIBBCM_HOST 
	#FLAGS  += -DUSE_EXTERNAL_LIBBCM_HOST
  #FLAGS  += -DHAVE_LIBOPENMAX=2
	FLAGS  += -DOMX
	FLAGS  += -DOMX_SKIP64BIT
	#FLAGS  += -DUSE_EXTERNAL_OMX
	FLAGS  += -DUSE_VCHIQ_ARM
	FLAGS  += -fPIC
	FLAGS  += -ftree-vectorize 
	FLAGS  += -pipe 	
	
	FLAGS  += -DHW_ODROIDC1

	# MALI on ODROID C1 specific, might be useful for other boards with Mali that need to run with X11
	# fbdev
	CPPDIR += /home/sirgraham/MALISDK-fbdev/inc
	LIBS	 += -L /home/sirgraham/MALISDK-fbdev/lib/arm/

	# X11
	#CPPDIR += /home/sirgraham/MALISDK/inc
	#LIBS	 += -L /home/sirgraham/MALISDK/lib/arm/

	CPPDIR += /usr/gcc-linaro-arm-linux-gnueabihf-raspbian/include
	#CPPDIR += /usr/gcc-linaro-arm-linux-gnueabihf-raspbian/include/interface/vcos/pthreads
	#CPPDIR += /usr/gcc-linaro-arm-linux-gnueabihf-raspbian/include/interface/vmcs_host/linux	
	
	CPPDIR += /usr/gcc-linaro-arm-linux-gnueabihf-raspbian/include/postgresql
	CPPDIR += /usr/arm-linux-gnueabihf/include/postgresql
	CPPDIR += /usr/arm-linux-gnueabihf/include
	
	

	LIBS	 += -L /usr/arm-linux-gnueabihf	
	LIBS	 += -L /usr/arm-linux-gnueabihf/lib	
	
	
	LIBS	 += -L /usr/gcc-linaro-arm-linux-gnueabihf-raspbian/lib
	#LIBS	 += -L /usr/gcc-linaro-arm-linux-gnueabihf-raspbian/lib/vc
	#LIBS	 += -L /usr/gcc-linaro-arm-linux-gnueabihf-raspbian/lib/ilclient
	
	LIBS	 += -L ../../../$(ADDDIRECTORYGEN)GEN/Platforms/Linux/MySQL
			
endif


#---- PINE 64 ------------------------------------------------------------------------------------------------

ifeq "$(ARCHITECTURE)" "PINE64"  
	CC		  = gcc
	CCPP	  = g++

	FLAGS	+= -DHW_PINE64
endif




#---- DEBUG MODE ------------------------------------------------------------------------------------------------

ifeq "$(DEBUGMODE)" "YES"
  FLAGS 		 += -g3 -Wno-psabi -rdynamic -funwind-tables -DXDEBUG_TRACE
  OBJECTSDIR	= ./debug
  FLAGSLINK   = 
 else
# FLAGS      += -g0 -Os -Wno-psabi 
 	FLAGS      += -g0  -Wno-psabi -rdynamic -funwind-tables 
  OBJECTSDIR  = ./release
  FLAGSLINK   = -s 
endif	      

#---- INCLUDES --------------------------------------------------------------------------------------------------

CPPDIR  += ../../../$(ADDDIRECTORYGEN)GEN/Cipher
CPPDIR  += ../../../$(ADDDIRECTORYGEN)GEN/Compress
CPPDIR  += ../../../$(ADDDIRECTORYGEN)GEN/DataIO
CPPDIR  += ../../../$(ADDDIRECTORYGEN)GEN/DataIO/I2C
CPPDIR  += ../../../$(ADDDIRECTORYGEN)GEN/DataIO/SPI
CPPDIR  += ../../../$(ADDDIRECTORYGEN)GEN/Graphic
CPPDIR  += ../../../$(ADDDIRECTORYGEN)GEN/Input
CPPDIR  += ../../../$(ADDDIRECTORYGEN)GEN/Interface
CPPDIR  += ../../../$(ADDDIRECTORYGEN)GEN/Main
CPPDIR  += ../../../$(ADDDIRECTORYGEN)GEN/Utils
CPPDIR  += ../../../$(ADDDIRECTORYGEN)GEN/Script
CPPDIR  += ../../../$(ADDDIRECTORYGEN)GEN/Sound
CPPDIR  += ../../../$(ADDDIRECTORYGEN)GEN/UserInterface
CPPDIR  += ../../../$(ADDDIRECTORYGEN)GEN/Video
CPPDIR	+= ../../../$(ADDDIRECTORYGEN)GEN/Media
CPPDIR  += ../../../$(ADDDIRECTORYGEN)GEN/ArtificialIntelligence
CPPDIR  += ../../../$(ADDDIRECTORYGEN)GEN/DatabasesSQL
CPPDIR  += ../../../$(ADDDIRECTORYGEN)GEN/DatabasesSQL/MySQL
CPPDIR  += ../../../$(ADDDIRECTORYGEN)GEN/DatabasesSQL/PostgreSQL
CPPDIR  += ../../../$(ADDDIRECTORYGEN)GEN/DatabasesSQL/SQLite
CPPDIR  += ../../../$(ADDDIRECTORYGEN)GEN/Platforms/Common/SNDOpenAL
CPPDIR  += ../../../$(ADDDIRECTORYGEN)GEN/Platforms/Common/VIDEOCodecH264
CPPDIR  += ../../../$(ADDDIRECTORYGEN)GEN/Platforms/Linux
CDIR	  += ../../../$(ADDDIRECTORYGEN)GEN/Physics

CPPDIR  += ../../../$(ADDDIRECTORYGEN)ThirdPartyLibraries/lua/src
CPPDIR  += ../../../$(ADDDIRECTORYGEN)ThirdPartyLibraries/openal-soft/include
CPPDIR  += ../../../$(ADDDIRECTORYGEN)ThirdPartyLibraries/libpcap

CDIR	  += ../../../$(ADDDIRECTORYGEN)ThirdPartyLibraries/zlib
CDIR	  += ../../../$(ADDDIRECTORYGEN)ThirdPartyLibraries/zlib/contrib/minizip
CDIR	  += ../../../$(ADDDIRECTORYGEN)ThirdPartyLibraries/jpeglib
CDIR	  += ../../../$(ADDDIRECTORYGEN)ThirdPartyLibraries/libpng
CDIR    += ../../../$(ADDDIRECTORYGEN)ThirdPartyLibraries/stb
CDIR	  += ../../../$(ADDDIRECTORYGEN)ThirdPartyLibraries/sqlite
CDIR	  += ../../../$(ADDDIRECTORYGEN)ThirdPartyLibraries/mysql-connector-c/include
CDIR	  += ../../../$(ADDDIRECTORYGEN)ThirdPartyLibraries/postgres/src/include
CDIR    += ../../../$(ADDDIRECTORYGEN)ThirdPartyLibraries/glew/include
CDIR    += ../../../$(ADDDIRECTORYGEN)ThirdPartyLibraries/glew/src
CDIR    += ../../../$(ADDDIRECTORYGEN)ThirdPartyLibraries/ffmpeg
								
INCLUDES += $(addprefix -I, $(CPPDIR))					 					 			   
INCLUDES += $(addprefix -I, $(CDIR))
           

#---- LIBS -------------------------------------------------------------------------------------------------------

ifneq (,$(findstring VIDEO_ACTIVE,$(FLAGS)))
	FLAGS += -D MEDIA_ACTIVE
endif

ifneq (,$(findstring SND_ACTIVE,$(FLAGS)))
	FLAGS += -D MEDIA_ACTIVE
endif

LIBS				+= -lstdc++
LIBS				+= -lpthread
LIBS				+= -lrt

ifneq (,$(findstring DIOBLUETOOTH_ACTIVE,$(FLAGS)))	
	LIBS			+= -l bluetooth
endif

ifneq (,$(findstring DIOWIFI_ACTIVE,$(FLAGS)))	
	LIBS			+= -l iw		
endif

ifneq (,$(findstring DIOUSB_ACTIVE,$(FLAGS)))	
	LIBS			+= -l usb-1.0 
	LIBS			+= -l udev 
endif

ifneq (,$(findstring DIOPCAP_ACTIVE,$(FLAGS)))	
	LIBS			+= -l pcap 
endif

ifneq (,$(findstring SND_ACTIVE,$(FLAGS)))
	LIBS                    += -l openal
endif

ifeq "$(ARCHITECTURE)" "PC"    
	ifneq (,$(findstring GRP_ACTIVE,$(FLAGS)))   
				
				LIBS				+=			-l X11
				LIBS				+= 			-l GL
        LIBS        +=			-l Xext
        LIBS        +=			-l Xxf86vm

	endif

#	ifneq (,$(findstring VIDEO_ACTIVE,$(FLAGS)))
#		LIBS	+=  -l swresample
# endif

	ifneq (,$(findstring MEDIA_ACTIVE,$(FLAGS)))
		LIBS	+=  -l avresample
	endif

	ifneq (,$(findstring DBSQL_ACTIVE,$(FLAGS)))
		LIBS	+=			-l dl	
	
		ifneq (,$(findstring DBSQLMYSQL_ACTIVE,$(FLAGS)))				
			LIBS	+=	-l mysqlclient
		endif		
		ifneq (,$(findstring DBSQLPOSTGRESQL_ACTIVE,$(FLAGS)))
			LIBS	+=	-l pq
		endif
	endif
endif


ifeq "$(ARCHITECTURE)" "RASPBERRYPI"
	ifneq (,$(findstring GRP_ACTIVE,$(FLAGS)))   
		LIBS		+= 	-l vcos
		LIBS		+= 	-l EGL
		LIBS		+= 	-l GLESv2    
		LIBS		+= 	-l bcm_host
		LIBS		+= 	-l vchiq_arm
		LIBS		+= 	-l openmaxil
		LIBS		+=	-l m
		LIBS		+= 	-l ilclient
		LIBS	+=			-l dl	
	endif
	
	ifneq (,$(findstring MEDIA_ACTIVE,$(FLAGS)))
		LIBS	+=  -l avresample
	endif

	ifneq (,$(findstring DBSQL_ACTIVE,$(FLAGS)))			
		LIBS	+=	-l dl	
				
		ifneq (,$(findstring DBSQLMYSQL_ACTIVE,$(FLAGS)))				
			LIBS	+=	-l mysqlclient_hf
		endif

		ifneq (,$(findstring DBSQLPOSTGRESQL_ACTIVE,$(FLAGS)))
			LIBS	+=	-l pq
			LIBS	+=	-l krb5
			LIBS	+=	-l ssl
			LIBS	+=	-l crypto
			LIBS	+=	-l com_err
			LIBS	+=	-l gssapi_krb5
			LIBS	+=	-l ldap_r-2.4
			LIBS	+=	-l k5crypto
			LIBS	+=	-l krb5support
			LIBS	+=	-l keyutils
			LIBS	+=	-l z
			LIBS	+=	-l lber-2.4
			LIBS	+=	-l sasl2
			LIBS	+=	-l gcrypt
			LIBS	+=	-l gpg-error
			LIBS	+=	-l gnutls-extra
			LIBS	+=	-l gnutls
			LIBS	+=	-l p11-kit
			LIBS	+=	-l tasn1			
		endif
	endif
endif           
           
ifeq "$(ARCHITECTURE)" "ODROIDC1"
	ifneq (,$(findstring GRP_ACTIVE,$(FLAGS)))   
		LIBS		+= 	-l EGL
		LIBS		+= 	-l GLESv2
		LIBS		+=	-l X11
		LIBS		+=  -l xcb   
		LIBS		+=  -l Xau
		LIBS		+=  -l Xdmcp
		LIBS		+=  -l Xxf86vm
		#LIBS		+=  -l Xext
	endif

	ifneq (,$(findstring DBSQL_ACTIVE,$(FLAGS)))	
		LIBS	+=	-l dl	
		ifneq (,$(findstring DBSQLMYSQL_ACTIVE,$(FLAGS)))				
			LIBS	+=	-l mysqlclient_hf
		endif
		ifneq (,$(findstring DBSQLPOSTGRESQL_ACTIVE,$(FLAGS)))
			LIBS	+=	-l pq
			LIBS	+=	-l krb5
			LIBS	+=	-l ssl
			LIBS	+=	-l crypto
			LIBS	+=	-l com_err
			LIBS	+=	-l gssapi_krb5
			LIBS	+=	-l ldap_r-2.4
			LIBS	+=	-l k5crypto
			LIBS	+=	-l krb5support
			LIBS	+=	-l keyutils
			LIBS	+=	-l z
			LIBS	+=	-l lber-2.4
			LIBS	+=	-l sasl2
			LIBS	+=	-l gcrypt
			LIBS	+=	-l gpg-error
			LIBS	+=	-l gnutls-extra
			LIBS	+=	-l gnutls
			LIBS	+=	-l p11-kit
			LIBS	+=	-l tasn1			
		endif
	endif
	
endif        
 
      
           
#---- ALL -------------------------------------------------------------------------------------------------------      

ifneq (,$(findstring XLIB,$(FLAGS)))	
	FLAGS		+= -fPIC
	FLAGSLINK	+= -fPIC -shared -Wl,-soname,$(TARGET).$(SONAMEVERSION)
endif

ifneq (,$(findstring MEDIA_ACTIVE,$(FLAGS)))   
		LIBS	+=	-l avcodec
		LIBS	+=	-l avformat
		LIBS	+=	-l avutil
endif

$(shell mkdir -p $(OBJECTSDIR) 2>/dev/null)

#HEADERS   	 = $(SOURCESCPP:.cpp=.h)
OBJECTS    	 = $(SOURCESCPP:.cpp=.o)
OBJECTS    	+= $(SOURCESC:.c=.o)
DEPENDENCIES = $(SOURCESCPP:.cpp=.d)
DEPENDENCIES += $(SOURCESC:.c=.d)

OBJECTSLINK  = $(patsubst %,$(OBJECTSDIR)/%,$(OBJECTS)) 
         
vpath %.h	$(CPPDIR)         
vpath %.cpp $(CPPDIR)
vpath %.c   $(CDIR)
vpath %.o   $(OBJECTSDIR)
                                                                                                                                                                            
all: $(TARGET) 
	@echo "  "
	@echo "Application  : $(TARGET)"
	@echo "Architecture : $(ARCHITECTURE)"
	@echo "Debug mode   : $(DEBUGMODE)"
	@echo "--------------------------------------------------------"
                                                           
%.o : %.c       
	@echo "Compiling $(*F).c ..."  	
	$(CC) $(FLAGS) $(FLAGSC) $(INCLUDES) -c -o $(OBJECTSDIR)/$(*F).o $<  
			                                                       
%.o : %.cpp %.h 	
	@echo "Compiling $(*F).cpp ..."
	$(CCPP) $(FLAGS) $(FLAGSCPP) $(INCLUDES) -c -o $(OBJECTSDIR)/$(*F).o $< 
			            
		           			                                                      
$(TARGET) : $(OBJECTS) $(HEADERS)
	@echo "Linking   $(TARGET)" 	
	$(CCPP) $(FLAGSLINK) $(FLAGS)  $(OBJECTSLINK) -Wl,--no-whole-archive -rdynamic -o $(TARGET) $(LIBS)


#---- CLEAN -----------------------------------------------------------------------------------------------------      
	                                                         
clean :                                                  
	rm -f $(TARGET) 
	rm -R $(OBJECTSDIR)

-include $(patsubst %,$(OBJECTSDIR)/%,$(DEPENDENCIES)) 
