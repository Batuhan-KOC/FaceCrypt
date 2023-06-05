1. Create a virtual environment.

2. Python version 3.6.0 is being used.

3. Activate venv.

3. Use following command to install requirements
pip install requirements.txt

4. Create a folder with given name "release"
mkdir release

5. Go to release directory
cd release

6. Convert python to exe with given command
pyinstaller --onedir ..\faceMatch.py

7. Copy cascade files located in "cascade" folder to following directory
release/dist/faceMatch/cv2/data

8. Use dist folder as release document
