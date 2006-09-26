@call "VC6vars.bat"
@cd "bin"
cmake -G "NMake Makefiles" -DCMAKE_BUILD_TYPE=Debug "../src"
@pause
