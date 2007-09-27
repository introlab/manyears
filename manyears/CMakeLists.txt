#Adding src subdirectory
SUBDIRS(src GUI)

SET(FFTW3_DLL
 C:/Dev-Cpp/bin/fftw3.dll
)
 
INSTALL(FILES ${FFTW3_DLL} DESTINATION bin)
