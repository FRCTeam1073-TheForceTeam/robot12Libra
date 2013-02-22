@echo off
cd ..
hg clone https://code.google.com/p/theforceteam-1073.dashboard12
hg pull
hg update
cls
cd theforceteam-1073.dashboard12
cd 2012FRC_1073Dash\2012FRC_1073Dash
cd bin
cls
tree /F /A
cd debug
copy 2012FRC_1073Dash.exe ..\..\..\..\..\theforceteam-1073.robotsim12
cd ..\..\..\..\..\theforceteam-1073.robotsim12