SilentInstall silent
AutoCloseWindow true
RequestExecutionLevel user

Name "GPS CLI"
OutFile "gps_cli.exe"

Section
  SetOutPath "$TEMP\gps_cli"
  File "gps_device_cli_example.exe"
  File "Qt5Core.dll"  
  File "Qt5SerialPort.dll"
  
  ExecWait '"$TEMP\gps_cli\gps_device_cli_example.exe" $CMDLINE'
  
  RMDir /r "$TEMP\gps_cli"
SectionEnd
