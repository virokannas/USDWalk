# USDWalk
Very simple SDL-based USD viewer with walk controls.

## Prerequisites
Current dependencies are [USD (Universal Scene Description)](https://github.com/PixarAnimationStudios/USD) libraries and
(optionally) Python if USD was compiled with Python support. If Python is
available, Python library paths are resolved using ```python_config``` which needs
to be in PATH
* [USD](https://github.com/PixarAnimationStudios/USD)
* [SDL2](https://www.libsdl.org/download-2.0.php)

## Building
First, you'll need to copy the SDL2.Framework bundle (not included) into the "Frameworks" folder.

There's a script that assists with building with XCode. It needs USD_ROOT to be set:

```shell
export USD_ROOT=/path/to/installed/USD
./setup_build.sh
```

After those steps, you should be able to build using xcodebuild:

```shell
xcodebuild
```

## Running
If USD libraries aren't installed in your system folders (most likely), you'll need to supply a path
to them when running:

```shell
LD_LIBRARY_PATH=$USD_ROOT/lib build/Release/USDWalk my_usd_file.usd
```

If you use the sample Kitchen_set.usd, you should be looking at something like this:

![Kitchen example](https://user-images.githubusercontent.com/38055305/44308387-db974500-a382-11e8-99c2-b0d77b6aabed.png)
