C:\Python27\Scripts\virtualenv venv
set PATH=%WORKSPACE%/venv/Scripts/;%PATH%

%WORKSPACE%/venv/Scripts/pip install -r requirements.txt
call build.bat
cd ..

%WORKSPACE%/venv/Scripts/python release.py