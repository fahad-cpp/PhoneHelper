# PhoneHelper

PhoneHelper is an scrcpy wrapper that i developed for my use. <br>
It is used for connecting an android device to a computer with a click <br>
currently it uses scrcpy as an external CLI tool but i will update the repo to install it automatically later 

# Building

```
mkdir build
cd build

cmake ..
cmake --build . --config Release -A x64 --parallel
```

# Usage
```
PhoneHelper.exe [--wireless/--usb] [--verbose (-v)]
```