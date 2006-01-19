CPP=cl.exe
RSC=rc.exe
LINK=link.exe

OUTDIR=bin
INTDIR=obj

CFLAGS=/nologo /MD /W3 /O2 /Fo"$(INTDIR)/" /c 
CPPFLAGS=/I"C:/Program Files/Microsoft Visual Studio 8/VC/PlatformSDK/Include"
CCFLAGS=$(CFLAGS) $(CPPFLAGS) 

LINK_LIBS=kernel32.lib shell32.lib user32.lib advapi32.lib credui.lib
LIBPATH=c:/Program Files/Microsoft Visual Studio 8/VC/PlatformSDK/Lib
LINK_FLAGS=/nologo /dll /libpath:"$(LIBPATH)" /opt:NOWIN98 /opt:REF /opt:ICF /machine:I386 /out:"$(OUTDIR)/Xutils.dll" $(LINK_LIBS)
LINK_OBJS= \
	"$(INTDIR)/resources.res" \
	"$(INTDIR)/RunAs.obj" \
	"$(INTDIR)/Wheel.obj" \
	"$(INTDIR)/Xutils.obj" \
	"$(INTDIR)/Eject.obj" \
	"$(INTDIR)/IdleTimers.obj"

RSCFLAGS=/fo"$(INTDIR)/resources.res"


ALL : "$(OUTDIR)/Xutils.dll"

CLEAN :
	-@erase "$(INTDIR)\Eject.obj"
	-@erase "$(INTDIR)\IdleTimers.obj"
	-@erase "$(INTDIR)\resources.res"
	-@erase "$(INTDIR)\RunAs.obj"
	-@erase "$(INTDIR)\Wheel.obj"
	-@erase "$(INTDIR)\Xutils.obj"
	-@erase "$(OUTDIR)\Xutils.dll"
	-@erase "$(OUTDIR)\Xutils.exp"
	-@erase "$(OUTDIR)\Xutils.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/" mkdir "$(INTDIR)"


"$(OUTDIR)/Xutils.dll" : "$(OUTDIR)" $(LINK_OBJS)
	$(LINK) @<<
	$(LINK_FLAGS) $(LINK_OBJS)
<<


.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CCFLAGS) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CCFLAGS) $< 
<<


SOURCE=Eject.c
"$(INTDIR)/Eject.obj" : $(SOURCE) "$(INTDIR)"

SOURCE=IdleTimers.c
"$(INTDIR)/IdleTimers.obj" : $(SOURCE) "$(INTDIR)"

SOURCE=RunAs.c
"$(INTDIR)/RunAs.obj" : $(SOURCE) "$(INTDIR)"

SOURCE=Wheel.c
"$(INTDIR)/Wheel.obj" : $(SOURCE) "$(INTDIR)"

SOURCE=Xutils.c
"$(INTDIR)/Xutils.obj" : $(SOURCE) "$(INTDIR)"

SOURCE=resources.rc
"$(INTDIR)/resources.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) $(RSCFLAGS) $(SOURCE)
