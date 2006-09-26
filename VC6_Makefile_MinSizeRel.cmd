@call "VC6vars.bat"
@cd "bin"
cmake -G "NMake Makefiles" -DCMAKE_BUILD_TYPE=MinSizeRel "../src"
@pause