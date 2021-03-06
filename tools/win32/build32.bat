@echo off
rem === Set all these variables to proper paths of your system:
call "C:\Program Files (x86)\Microsoft Visual Studio 9.0\VC\bin\vcvars32.bat" x86

set QTDIR=c:\Programs\Qt\4.7.0-beta2

set BOOST_ROOT=f:\X-Files\Projects\Lib\boost_1_43_0

set TORRENT_DIR=f:/X-Files/Projects/Lib/libtorrent/tags/libtorrent-0_15_1
set TORRENT_INCLUDE_DIR=%TORRENT_DIR%/include
set TORRENT_LIBRARY=%TORRENT_DIR%/bin/msvc-9.0/release/boost-link-shared/boost-source/threading-multi/torrent.lib

set PYTHONDIR=c:/Programs/Python27
set PYTHON_INCLUDE_DIR=%PYTHONDIR%/include
set PYTHON_LIBRARY=%PYTHONDIR%/libs/python27.lib

rem Be sure that cmake executable is in your system %PATH%.
if exist build32 del /f /s /q build32
if not exist build32 mkdir build32
cd build32
rem cmake ../../../src -G "Visual Studio 9 2008" -DCMAKE_BUILD_TYPE=Release -DRBTorrent_INCLUDE_DIR=%TORRENT_INCLUDE_DIR% -DRBTorrent_LIBRARY=%TORRENT_LIBRARY% -DCURL_INCLUDE_DIR=%CURL_INLUDE_DIR% -DCURL_LIBRARY=%CURL_LIBRARY% -DBZIP2_INCLUDE_DIR=%BZIP2_INCLUDE_DIR% -DBZIP2_LIBRARIES=%BZIP2_LIBRARY% -DPYTHON_INCLUDE_DIR=%PYTHON_INCLUDE_DIR% -DPYTHON_LIBRARY=%PYTHON_LIBRARY%
cmake ../../../src -G "Visual Studio 9 2008" -DCMAKE_BUILD_TYPE=Release -DRBTorrent_INCLUDE_DIR=%TORRENT_INCLUDE_DIR% -DRBTorrent_LIBRARY=%TORRENT_LIBRARY% -DENABLE_DCMINATOR=False -DENABLE_PYLC=False -DENABLE_LCFTP=False -DENABLE_QROSP=True -DPYTHON_INCLUDE_DIR=%PYTHON_INCLUDE_DIR% -DPYTHON_LIBRARY=%PYTHON_LIBRARY%
pause