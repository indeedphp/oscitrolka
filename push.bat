del circuit\kicad\carcontroltool-nano-plan\carcontroltool-esp32-plan.xml
del circuit\kicad\carcontroltool-nano-plan\*.lck
del circuit\kicad\carcontroltool-nano-plan\carcontroltool-esp32-plan-backups\*
@if "%1" EQU "" (
@echo !!! Add a comment like: push.bat "test x" 
exit
)
git add *
git commit -m %1
git push origin main
