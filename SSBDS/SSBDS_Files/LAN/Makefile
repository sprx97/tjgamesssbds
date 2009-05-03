# PALib Makefile by Scognito and Tom and crash

#---------------------------------------------------------------------------------
# Please uncomment (i.e. delete the '#') only one "ARM7_SELECTED" line. If unsure,
# just uncomment "ARM7_SELECTED = ARM7_BASIC". ARM7_BASIC is the same as ARM7_MP3.
#---------------------------------------------------------------------------------

#ARM7_SELECTED = ARM7_BASIC
#ARM7_SELECTED = ARM7_MOD_DSWIFI
#ARM7_SELECTED = ARM7_MOD_LIBLOBBY
#ARM7_SELECTED = ARM7_MP3
#ARM7_SELECTED = ARM7_MP3_DSWIFI
ARM7_SELECTED = ARM7_MP3_LIBLOBBY
#ARM7_SELECTED = ARM7_MIKMOD_DSWIFI

#---------------------------------------------------------------------------------
# If you're using EFS_lib, uncomment "USE_EFS = YES" here.
#---------------------------------------------------------------------------------

#USE_EFS = YES

PATH := $(DEVKITARM)/bin:$(PATH)
PROGNAME = PAlib
OFILES +=
ADD_LIBS +=

#---------------------------------------------------------------------------------
# Be sure to change these default banner TEXTs. This is the name your project will
# display in the DS menu (including some flash cards). Create a custom LOGO.BMP
# icon for your project, too!
#---------------------------------------------------------------------------------

TEXT1 	:= PAlib Project -
TEXT2 	:= Change this text
TEXT3 	:= for your project!
ICON 	:= -b $(CURDIR)/../logo.bmp

#---------------------------------------------------------------------------------
# TARGET is the name of the file to output
# BUILD is the directory where object files and intermediate files will be placed
# SOURCES is a list of directories containing source code
# INCLUDES is a list of directories containing extra header files
#---------------------------------------------------------------------------------

TARGET		:= $(shell basename $(CURDIR))
BUILD		:= build
SOURCES		:= gfx source data
INCLUDES	:= include build data

#---------------------------------------------------------------------------------
# If you need to change other parts of the make process, see the PA_Makefile:
#---------------------------------------------------------------------------------

include $(PAPATH)/PA_Makefile

