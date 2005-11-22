LIBS = kernel32.lib shell32.lib user32.lib advapi32.lib credui.lib
LIBPATH = c:\Program Files\Microsoft Platform SDK\Lib
INCLUDEPATH = c:\Program Files\Microsoft Platform SDK\Include
OBJS = Eject.obj IdleTimers.obj Wheel.obj RunAs.obj Xutils.obj

all: DLL clean

DLL: Xutils.dll

Xutils.dll: $(OBJS)
	link /nologo /dll /libpath:"$(LIBPATH)" /opt:NOWIN98 /out:Xutils.dll $(OBJS) $(LIBS)

.c.obj::
	cl /nologo /MD /W3 /O2 /TC /I"$(INCLUDEPATH)" /c $<

clean:
	@del *.obj
	@del *.lib
	@del *.exp
