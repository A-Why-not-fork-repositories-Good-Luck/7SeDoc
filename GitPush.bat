@echo off

git add *

set /p MESSSAGE="Commit Message: "

git commit -m "%MESSSAGE%"

git push

echo Your Commit Message is: %MESSSAGE%

pause