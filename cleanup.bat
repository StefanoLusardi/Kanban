for /d /r %%D in (GeneratedFiles, x64) DO rd /s /q "%%D"
pause
exit
