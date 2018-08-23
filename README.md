# Arduino Controller
### [Releases](https://github.com/Sateviss/arduino-controller/releases)
Software for using Arduino as an external control board

The system is designed to receive signals from Arduino boards via a serial connection and to run *Python 3* scripts accordingly. Using 
[PyAutoGUI](https://pyautogui.readthedocs.io/en/latest/) allows you to quickly *(~200 ms)* simulate keystrokes or mouse movements to
automate mundane tasks.

## Quickstart tutorial
- Build a simple circuit with an Arduino, using some kind of switch *(some kind of button or a toggle switch)*
to contol the flow of current.
- Connect it to the computer via USB.
- Run the **Arduino Controller** *(you must have proper rights to access serial ports on Linux)*.
- Select the proper port from the settings menu at the top `Settings -> Port`.
- Select the pin you want to set up.
- Select when you want to run the script: 
  - `Any state change` is good to simulate key presses when a toggle switch is flipped.
  - `LOW to HIGH` means that the script will run when the key is initially pressed.
  - `HIGH to LOW` means that the script will run when the key is released.
- Select the proper action type:
  - `Run a script` will run a custom Python script. 
  This option will require following actions:
    - Select where you want to run the script i.e.:
      - `separate interpreter` will run the script in a separate interpreter. Good for large complicated scripts.
      - `global interpreter` will stream the script into a global *Python* interpreter. Good for actions that need to be executed quickly.
    - Write/Paste/Load a *Python* script in editng field.
  - `Press keys` will automatically create a script that simulates pressing keys.
  This option will require following actions:
    - Add all the keys you want to simulate: `Settings -> Add or replace a key...`.
    - Press the `Key combination` button, then press the key combination you want to simulate on your keyboard.
- After making all the changes to the pin press `Accept` to save all changes or `Reset` to discard them.

## System compatibility 
- Linux (Debian 10) - fully operational
- Windows 10 (1803) - fully operational
- OS X (Mojave 10.14) - serial ports do not work

The release comes packaged with a Python 3.6 enviorment with PyAutoGUI installed
