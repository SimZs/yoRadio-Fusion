Edited by: W76W - 2025.10.26., 14:55
Source: https://4pda.to/forum/index.php?act=search&source=pst&result=posts&sort=rel&subforums=0&query=liblwip&topics[]=1010378&forums=89&st=100

Regarding the liblwip libraries belonging to version 3.3.3 of the latest IDF 5.5 framework,
I have compiled different versions for those who want to experiment.
Three versions with different settings.

idf.py menuconfig --> (Top) → Component config → LWIP → TCP 
																		Eredeti				Könnyű		Közepes		Extrém
Maximum active TCP Connections		CONFIG_LWIP_MAX_ACTIVE_TCP=			16(1-1024)			64			128			512
Default receive window size 		CONFIG_LWIP_TCP_WND_DEFAULT=		5760(2440-65635)	32768		49152		65535
Default send buffer size 			CONFIG_LWIP_TCP_SND_BUF_DEFAULT=	5760(2440-65535)	8192		16384		32768
TCPIP task receive mail box size	CONFIG_LWIP_TCP_RECVMBOX_SIZE=		32(6-32)			32			48			64
Maximum listening TCP Connections	CONFIG_LWIP_MAX_LISTENING_TCP=		16(1-1024)

I liked the middle one better, although I realized that these two parameters have the greatest influence:
CONFIG_LWIP_TCP_WND_DEFAULT=32768
CONFIG_LWIP_TCP_RECVMBOX_SIZE=32

translated into the current and stable version.
framework = arduino
platform = https://github.com/pio…atform-espressif32.zip

**** Names of files to be replaced ****
libesp_netif.a
liblwip.a


**** It works well under PlatformIO with these settings: ****
[env:esp32-s3-devkitc1-n16r8]
;Arduino Release v3.3.3 based on ESP-IDF v5.5.1.251017
platform = https://github.com/pioarduino/platform-espressif32/releases/download/55.03.33/platform-espressif32.zip 
board = esp32-s3-devkitc1-n16r8
framework = arduino

Location of lib files on the computer:
C:\Users\tvara\.platformio\packages\framework-arduinoespressif32-libs\esp32s3\lib


**** Under Arduino_IDE 1.8.19 ****
Location of lib files on the computer:
C:\Users\USERNAME\AppData\Local\Arduino15\packages\esp32\tools\esp32-arduino-libs\idf-release_v5.5-f1a1df9b-v3\esp32s3\lib