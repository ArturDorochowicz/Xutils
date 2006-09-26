@call "VC6vars.bat"
cmake -G "NMake Makefiles" -DCMAKE_BUILD_TYPE=MinSizeRel "../src"
@pause