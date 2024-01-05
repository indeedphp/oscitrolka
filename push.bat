@del /F /Q circuit\kicad\carcontroltool-esp32-plan\carcontroltool-esp32-plan.xml > nul 2>&1
@del /F /Q circuit\kicad\carcontroltool-esp32-plan\*.lck > nul 2>&1
@del /F /Q circuit\kicad\carcontroltool-esp32-plan\carcontroltool-esp32-plan-backups\* > nul 2>&1
@if '%1' EQU '' (
@echo !!! Add a comment like: push.bat "test x" 
exit
)
git add *
git commit -m %1
git push origin esp32
