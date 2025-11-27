@echo off
echo Starting The Game Server and Clients...
cd x64\Debug

echo Launching Server...
start "The Game SERVER" Server.exe

echo Waiting for server to initialize...
timeout /t 2 >nul

echo Launching Player 1...
start "Player 1" ClientConsole.exe

echo Launching Player 2...
timeout /t 1 >nul
start "Player 2" ClientConsole.exe

echo Done! Check the opened windows.
pause
