@echo off
rem Local preview of the docs site on Windows. Double-click it, or from a terminal:
rem   run.cmd            live preview at http://localhost:4321/
rem   run.cmd --build    full production build, served from dist/
rem Needs Node.js 20+ and Doxygen; scripts\run.mjs checks both and installs the npm packages.
setlocal
cd /d "%~dp0"
where node >nul 2>nul
if errorlevel 1 (
	echo Node.js 20 or newer is needed: winget install OpenJS.NodeJS.LTS, or https://nodejs.org/
	pause
	exit /b 1
)
node scripts\run.mjs %*
set RC=%errorlevel%
rem run.mjs exits with 2 when a check, the install or the build failed. Keep the window
rem open so a double-clicked run still shows why. Stopping with Ctrl+C is not a failure.
if %RC% equ 2 pause
exit /b %RC%
