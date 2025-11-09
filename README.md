# 🎺 AIRHORN 🎺

App for macOS & linux that just plays the airhorn noise when you click it.

## MacOS

### Download
Unzip the latest Airhorn.app release from `/releases` and live your airhorn-fueled dreams.

![Screenshot](docs/screenshot_1.png)

### Usage
- Left-click the trumpet icon to make the airhorn noise.
- Right-click the trumpet icon to close the app, removing it from the menu bar.

### Building from Source
```bash
# Quick build
cd macOS
./build_release.sh

# Or manually with Xcode
cd macOS
xcodebuild -project Airhorn.xcodeproj -scheme Airhorn -configuration Release build
```

**System Requirements:** macOS 11.0 or later (tested on macOS 26 Tahoe)

## Linux

- `cd airhorn/linux/xfce4`
- `./autogen.sh --prefix=/usr`
- `make`
- `sudo make install`

Check out the [docs](linux/xfce4) for more about linux support.
