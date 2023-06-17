@echo off

set commit_message=Saving games

REM Add all files
git add -A

REM Commit changes
git commit -m "%commit_message%"

REM Push changes to remote repository
git push

pause