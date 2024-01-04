del carcontroltool-esp32-plan.xml
del *.lck
del carcontroltool-esp32-plan-backups\*
@if '%1' EQU '' (
@echo !!! Add a comment like: push.bat "test x" 
exit
)
git add *
git commit -m %1
git push origin esp32
