# GEN Third Party Libraries

This is the list of third-party libraries used by the GEN framework.

## List of third party libraries

Name                                                                       | Version  
-----------------------------------------------------------------------    | ------------  
[**Zlib**](#zlib)                                                          | 1.2.11      
[**LibPNG**](#libpng)                                                      | 1.6.34        
[**jpeglib**](#jpeglib)                                                    | 9c
[**Lua**](#lua)                                                            | 5.3.4
[**STB Vorbis**](#stb)                                                     | 2.31 
[**OpenAL Soft**](#openal-soft)                                            | 1.18.2
[**libpcap**](#libpcap)                                                    | 1.8.1
[**SQLite**](#sqlite)                                                      | 3.23.1
[**MySQL Connector C**](#mysql-connector-c)                                | 6.1.11
[**LibPQ**](#libpq)                                                        | 
[**FFmpeg**](#ffmpeg)                                                      | 4.0
[**OpenGL-Registry**](#opengl-registry)                                    | 
[**OpenGL Extension Wrangler Library**](opengl-extension-wrangler-library) | 1.13.0
[**InpOut 32/64**](inpout-32/64)                                           | 1.5.0.1
[**STM32Fxxx HAL**](stm32fxxx-hal)                                         | 
[**Physx**](physx)                                                         | 3.4    


## Details of third party libraries


### **Zlib**

A Massively Spiffy Yet Delicately Unobtrusive Compression Library. It was written by Jean-loup Gailly (compression) and Mark Adler (decompression).

Directory        **zlib**  
Version          **1.2.11**    
Web              **https://zlib.net/**  
Download URL     **https://github.com/madler/zlib**  
Comments         
You must use the command ./configure in linux.  GEN use also some .H + .C of the subdirectory contrib/minicom. 



### **LibPNG**  

LibPNG is the official PNG reference library. It supports almost all PNG features, is extensible, and has been extensively tested for over 22 years. Libpng is available as ANSI C (C89) source code and requires zlib 1.0.4 or later.

Directory        **libpng**  
Version          **1.6.34**  
Web              **http://www.libpng.org/pub/png/libpng.html**  
Download URL     **https://sourceforge.net/projects/libpng/files/libpng16/**  
Comments         
You must use the command ./configure in linux and eliminate some #defines of pngconf.h



### **jpeglib**  

This library is distributes by IJG (is an informal group). The first version was released on 7-Oct-1991.This is a stable and solid foundation for many application's JPEG support.

Directory        **jpeglib**  
Version          **9c**  
Web              **http://www.ijg.org**  
Download URL     **http://www.ijg.org/files/jpegsr9c.zip**  
Comments         **Create jconfig.h**  



### **Lua** 

Lua is a powerful, efficient, lightweight, embeddable scripting language. It supports procedural programming, object-oriented programming, functional programming, data-driven programming, and data description.

Directory        **lua**  
Version          **5.3.4**  
Web              **https://www.lua.org**  
Download URL     **https://www.lua.org/download.html**   
Comments  



### **STB**  

Collection of libraries for C/C++- Most libraries by stb, except: stb_dxt by Fabian "ryg" Giesen, stb_image_resize by Jorge L. "VinoBS" Rodriguez, and stb_sprintf by Jeff Roberts.

Directory        **stb**  
Version          **2.31**  
Web              **http://nothings.org/stb_vorbis/**  
Download URL     **https://github.com/nothings/stb**  
Comments         
Need add stb_vorbis.h (OGG file format) to GEN compile. 



### **OpenAL Soft**

OpenAL provides capabilities for playing audio in a virtual 3D environment. Distance attenuation, doppler shift, and directional sound emitters are among the features handled by the API. More advanced effects, including air absorption, occlusion, and environmental reverb, are available through the EFX extension. It also facilitates streaming audio, multi-channel buffers, and audio capture.

Directory        **openal-soft**  
Version          **1.18.2**  
Web              **http://openal-soft.org/**  
Download URL     **https://github.com/kcat/openal-soft**  
Comments
It´s neccesary download the binary files. Used dinamic librarys to run GEN applications.



### **libpcap**

The Packet Capture library provides a high level interface to packet capture systems. All packets on the network, even those destined for other hosts, are accessible through this mechanism. It also supports saving captured packets to a savefile, and reading packets from a savefile.

Directory        **libpcap**  
Version          **1.8.1**  
Web              **http://www.tcpdump.org**  
Download URL     **https://github.com/the-tcpdump-group/libpcap**  
Comments
It´s neccesary download the binary files. Used dinamic librarys to run GEN applications.




### **SQLite**

SQLite is a self-contained, high-reliability, embedded, full-featured, public-domain, SQL database engine. SQLite is the most used database engine in the world. 

Directory        **sqlite**  
Version          **3.23.1**  
Web              **https://www.sqlite.org/index.html**  
Download URL     **https://www.sqlite.org/download.html**   
Comments



### **MySQL Connector C**

Connector/C (libmysqlclient) is a client library for C development of MySQL.

Directory        **mysql-connector-c**  
Version          **6.1.11**  
Web              **https://www.mysql.com**  
Download URL     **https://dev.mysql.com/downloads/connector/c/** 
Comments
It´s neccesary download the binary files. Used dinamic librarys to run GEN applications.



### **LibPQ**
LibPQ is part of the official PostgreSQL GIT repository. It´s the interface/connector for this SQL database.

postgres Connector  
Directory        **postgres**  
Version          **N/A**  
Web              **https://www.postgresql.org/**  
Download URL     **https://github.com/postgres/postgres**   
Comments
It´s neccesary download the binary files. Used dinamic librarys to run GEN applications.



### **FFmpeg**

FFmpeg is the leading multimedia framework, able to decode, encode, transcode, mux, demux, stream, filter and play pretty much anything that humans and machines have created. It supports the most obscure ancient formats up to the cutting edge. 

Directory        **ffmpeg**  
Version          **4.0**  
Web              **http://ffmpeg.org  https://www.ffmpeg.org/download.html#repositories**  
Download URL     **https://ffmpeg.zeranoe.com/builds/**  
Comments
It´s neccesary download the binary files. Used dinamic librarys to run GEN applications.



### **OpenGL-Registry**

API and Extension registries for the OpenGL family APIs - OpenGL, OpenGL ES, and OpenGL SC. It includes API specifications; specifications of Khronos- and vendor-approved extensions; header files corresponding to the specifications; the XML API Registry definining each API; and related tools and scripts.

Directory        **OpenGL-Registry**  
Version          **Unknown**  
Web              **https://www.khronos.org/registry/OpenGL/**  
Download URL     **https://github.com/KhronosGroup/OpenGL-Registry**  
Comments
Only the header glext.h is used.



### **OpenGL Extension Wrangler Library**

The OpenGL Extension Wrangler Library (GLEW) is a cross-platform open-source C/C++ extension loading library. GLEW provides efficient run-time mechanisms for determining which OpenGL extensions are supported on the target platform. OpenGL core and extension functionality is exposed in a single header file. 

Directory        **glew**   
Version          **1.13.0**  
Web              **http://glew.sourceforge.net/**  
Download URL     **https://github.com/nigels-com/glew**  
Comments         
It is necessary to modify the file glew.h. 



### **InpOut 32/64**

InpOut32 is an open source windows DLL and Driver to give direct access to hardware ports (for example the parallel and serial port from user level programs. It is originally developed by the people at Logix4U for Windows 9x and 32bit variations of NT (NT/2000/XP/2003 etc.).

Directory        **InpOutBinaries**  
Version          **1.5.0.1**  
Web              **http://www.highrez.co.uk/**  
Download URL     **http://www.highrez.co.uk/downloads/inpout32/**  
Comments         
Only for Windows platform.



### **STM32Fxxx HAL**

Hardware Abstraccion Layer librarys for STM32Fxxx microcontrolers.

Directory        **STM32Fxxx**  
Version          **N/A**  
Web               **http://www.st.com/en/microcontrollers/stm32-32-bit-arm-cortex-mcus.html**  
Download URL     **http://stm32f4-discovery.net/hal_api/**   
Comments         
This librarys are create with a tool: STM32Cube 



### **Physx**

PhysX is a scalable multi-platform game physics solution supporting a wide range of devices, from smartphones to high-end multicore CPUs and GPUs. 

Directory        **physx**  
Version          **3.4** 
Web              **https://developer.nvidia.com/gameworks-physx-overview**  
Download URL     **https://github.com/NVIDIAGameWorks/PhysX-3.4**  
Comments
Only for Windows platform.











