---
url: https://lvgl.io/docs/pro/editor/installation
title: Installation
---

# Installation

Source: <https://lvgl.io/docs/pro/editor/installation>

[Editor](https://lvgl.io/docs/pro/editor)

# Installation
Install LVGL Pro Editor on Windows, macOS, and Linux with platform-specific setup instructions and troubleshooting.
Copy MarkdownOpen
Get LVGL Pro Editor running on your system with installation steps tailored to your operating system.

## Windows

### Install WSL

The Editor requires **Windows Subsystem for Linux** (WSL) to run on Windows.

First, check if WSL is already installed by opening a Terminal and running:

bash

```
wsl.exe --list --verbose
```

If WSL is not installed, run:

bash

```
wsl --install
```

Then press Enter and wait for the installation to complete.

### Install the Editor

- Go to [pro.lvgl.io/#download](https://pro.lvgl.io/#download)

- Click the Download button

- Save the installer to your computer

- Run the installer and follow the prompts to complete the installation

## Linux

### Install Podman

The Editor compiles and runs C code in containers using **Podman** (similar to Docker). Install Podman for your Linux distribution:

- Debian, Ubuntu, Linux Mint: `sudo apt-get install podman`

- Fedora: `sudo dnf -y install podman`

- Arch, Manjaro Linux: `sudo pacman -S podman`

For other distributions, see the [Podman installation guide](https://podman.io/docs/installation#installing-on-linux).

### Install the Editor

- Download the AppImage from [pro.lvgl.io/#download](https://pro.lvgl.io/#download)

- Make it executable:
bash
```
chmod +x LVGL_Pro_Editor.AppImage
```

- Run the application:
bash
```
./LVGL_Pro_Editor.AppImage
```

### Troubleshooting

**Issue:** Double-clicking the `.AppImage` file doesn't launch the application, or you see a GTK error:

bash

```
(process:3943): Gtk-ERROR **: 11:45:01.301: GTK 2/3 symbols detected.
Using GTK 2/3 and GTK 4 in the same process is not supported
Trace/breakpoint trap (core dumped)
```

**Solution:** Run the AppImage from the terminal with the GTK version specified:

bash

```
./LVGL_Pro_Editor.AppImage --gtk-version=3
```

## MacOS

### Install Podman

The Editor compiles and runs C code in containers using **Podman** (similar to Docker). Install it using Homebrew:

bash

```
brew install podman
```

For more information, see the [Podman installation guide for macOS](https://podman.io/docs/installation#macos).

### Install the Editor

- Download the appropriate version from [pro.lvgl.io/#download](https://pro.lvgl.io/#download):

ARM for Apple Silicon (M1/M2/M3)

- AMD for Intel-based Macs

- Drag the application to your Applications folder in Finder

- Launch the Editor from Applications or Spotlight

Last updated on
[Editor OverviewDiscover LVGL Pro Editor features including XML editing, real-time preview, code generation, and widget support.](https://lvgl.io/docs/pro/editor/overview)[User InterfaceExplore the LVGL Pro Editor interface, including the project files pane, XML editor, live preview, and inspector tools for building embedded UIs.](https://lvgl.io/docs/pro/editor/user-interface)
