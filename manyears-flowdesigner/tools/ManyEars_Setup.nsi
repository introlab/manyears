; example1.nsi
;
; This script is perhaps one of the simplest NSIs you can make. All of the
; optional settings are left to their default settings. The installer simply 
; prompts the user asking them where to install, and drops a copy of example1.nsi
; there. 

;--------------------------------

; The name of the installer
Name "ManyEars FlowDesigner plugin"

; The file to write
OutFile "ManyEars-0.2.exe"

; The default installation directory
InstallDir "C:\Program Files\FlowDesigner"

;--------------------------------

; Pages

Page directory
Page instfiles

;--------------------------------

; The stuff to install


Section "binaries" ;No components page, name is not important

  ; Set output path to the installation directory.
  SetOutPath $INSTDIR\bin
  
  ; Put file there
  File bin\audioviewer.exe
  File bin\libfftw3-3.dll
  File bin\libfftw3f-3.dll
SectionEnd ; end the section


Section "toolbox-def" ;No components page, name is not important

  ; Set output path to the installation directory.
  SetOutPath $INSTDIR\lib\flowdesigner\toolbox\ManyEars\def
  
  ; Put file there       
  File /r lib\flowdesigner\toolbox\ManyEars\def\* 
 	
SectionEnd ; end the section

Section "toolbox-lib" ;No components page, name is not important

  ; Set output path to the installation directory.
  SetOutPath $INSTDIR\lib\flowdesigner\toolbox\ManyEars\lib
  
  ; Put file there       
  File /r lib\flowdesigner\toolbox\ManyEars\lib\* 
 	
SectionEnd ; end the section

Section "toolbox-examples" ;No components page, name is not important

  ; Set output path to the installation directory.
  SetOutPath $INSTDIR\lib\flowdesigner\toolbox\ManyEars\examples
  
  ; Put file there       
  File /r lib\flowdesigner\toolbox\ManyEars\examples\* 
 	
SectionEnd ; end the section
