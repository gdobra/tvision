SHARED = 0
DEBUG = 0

CC = gcc
CXX = g++
LD = $(CXX)
INCSWITCH = -I
CCFLAGS = -Wall -c $(INCDIR)
CXXFLAGS = -Wall -c $(INCDIR)
CXXOUT = -o
CCOUT = -o
LINKLIB = -l
LINKFLAGS = -shared $(LINKLIB)user32 $(LINKLIB)kernel32
rm = del
mkdir = md
mv = move


ifeq ($(DEBUG),1)
CXXFLAGS += -g
CCFLAGS += -g
else
CXXFLAGS += -s -O3
CCFLAGS += -s -O3
LINKFLAGS += -Wl,--strip-all
endif

include files.mk

BASEDIR = ..\..
SRCDIR = $(BASEDIR)\src
INCDIR = $(INCSWITCH)$(BASEDIR)\INCLUDE $(INCSWITCH)$(BASEDIR)\INCLUDE\TVISION
LIBDIR = $(BASEDIR)\LIB
OUTDIR = $(BASEDIR)\lib\$(BUILDDIR)

IMPLIB = $(addprefix $(OUTDIR)\, libtvision.a)

ifeq ($(SHARED),1)
LIBNAME = $(OUTDIR)\TVISION.DLL
BUILDLIB = $(LD) $(LINKFLAGS) $(addprefix $(BUILDDIR)\, $(OBJS)) -Wl,--out-implib="$(IMPLIB)" -o "$(LIBNAME)"
BUILDDIR = gcc_dll
else
#LIBNAME = $(IMPLIB)
BUILDLIB = ar rcus $(IMPLIB) $(addprefix $(BUILDDIR)\, $(OBJS))
BUILDDIR = gcc_lib
CCFLAGS +=  -ffunction-sections -fdata-sections
CXXFLAGS += -ffunction-sections -fdata-sections -static  
endif

vpath %.cpp $(SRCDIR)
vpath %.h $(INCDIR)
vpath %.obj $(BUILDDIR)

all: $(SRCDIR) $(CPPS) $(BUILDDIR) $(OBJS) $(OUTDIR) $(LIBNAME) $(IMPLIB)

$(SRCDIR):
	$(mkdir) $@
	$(error Cannot find source directory $(SRCDIR))

$(CPPS):
	$(error Some cpp files are missing; check directory $(SRCDIR) against files listed in files.mk)

$(BUILDDIR) $(OUTDIR):
	@ echo Creating directory $@
	$(mkdir) $@

%.obj: %.cpp
	@ echo Compiling file $@ from $?
	$(CXX) $(CXXFLAGS) $? $(CXXOUT) $(addprefix $(BUILDDIR)\, $@)

$(IMPLIB) $(LIBNAME): $(OUTDIR) $(OBJS)
	@ echo Building library $(LIBNAME)
	$(BUILDLIB)
	ranlib $(IMPLIB)

.PHONY clean:
	$(rm) $(addprefix $(BUILDDIR)\, *.obj)
