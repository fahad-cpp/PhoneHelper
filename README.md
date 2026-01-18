# PhoneHelper

PhoneHelper is an scrcpy wrapper that i developed for my use. <br>
It is used for connecting an android device to a computer with a click <br>
currently it uses scrcpy as an external CLI tool but i will update the repo to install it automatically later 

# Building

```
mkdir build
cd build

cmake .. -G "Visual Studio 17 2022" -DCMAKE_BUILD_TYPE=Release -A x64
cmake --build . --config Release --parallel
```

# Usage
```
PhoneHelper.exe [--wireless/--usb] [--verbose (-v)]
```

--wireless : chooses wireless connection over usb connection and tries to make a wireless connection if specified

--usb : chooses USB connection over wireless connection