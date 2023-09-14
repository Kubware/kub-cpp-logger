echo off

Rem Do not forget to compile solution for Debug and Release

echo Do not forget to rebuild both DEBUG and RELEASE before running!


Rem This batchfile detects version info from logger.h and use it in nuget package version.
setlocal enabledelayedexpansion

Rem detect version info

set findtext="LOGGER_VERSION"
set findfile="src\logger.h"
set version=0.0.0

for /f "delims=" %%a in ('findstr %findtext% %findfile%') do set version=%%a
	set version=%version:"=%
	set version=%version:#define LOGGER_VERSION =%
	
Rem show version number

echo Current version is %version%


Rem generate NuGet with correct version
 
nuget pack nuget/KubCppLogger.nuspec -outputDirectory C:/NuGet -verbosity detailed -version %version%

echo REMINDER: Rebuild both DEBUG and RELEASE before running this batchfile...
