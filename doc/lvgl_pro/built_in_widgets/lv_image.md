---
url: https://lvgl.io/docs/pro/built_in_widgets/lv_image
title: Image (lv_image)
---

# Image (lv_image)

Source: <https://lvgl.io/docs/pro/built_in_widgets/lv_image>

[Built-in Widgets](https://lvgl.io/docs/pro/built_in_widgets)

# Image (lv_image)
Displays images from arrays or files, with recolor, scale and rotation.
Copy MarkdownOpen

## Overview

The Image widget displays images from compiled-in arrays or from files. It can recolor, scale, rotate and tile its source, and supports custom formats through the image decoder interface.

Learn more in the [LVGL Open documentation](https://lvgl.io/docs/open/widgets/image).

PreviewCXML
[lv_example_image.c](https://github.com/lvgl/lvgl_editor/blob/master/docs/examples/screens/lv_example_image.c)

## Enumerations

**`lv_image_align`** — How the image is placed inside its container

| Value | Description |
| --- | --- |
| `default` | Align top-left (same as top_left) |
| `top_left` | Align to top-left |
| `top_mid` | Align to top-center |
| `top_right` | Align to top-right |
| `bottom_left` | Align to bottom-left |
| `bottom_mid` | Align to bottom-center |
| `bottom_right` | Align to bottom-right |
| `left_mid` | Align to left-center |
| `right_mid` | Align to right-center |
| `center` | Align to center |
| `stretch` | Stretch to fill the widget |
| `tile` | Tile to fill the widget |
| `contain` | Scale to fit inside while keeping aspect ratio |
| `cover` | Scale to fully cover while keeping aspect ratio |

## Properties

### Parts

Style parts of the widget with local style (e.g. `style_bg_color-knob="0xff0000"`) or with style sheets (e.g. `<style name="style_knob" selector="knob">`). See [Styles](https://lvgl.io/syntax/styles) to learn more.

| Part | Description |
| --- | --- |
| `main` | Style the image: `image_recolor`, `image_recolor_opa`, `image_opa` and the transform properties (scale/rotation). Background, border, etc can be added too. |

Properties below are the widget's XML [`<api>`](https://lvgl.io/syntax/api); see [API](https://lvgl.io/syntax/api) for how properties, parameters and elements work.

| Property | Type | Description |
| --- | --- | --- |
| `src` | `image` | Set the image source (file, symbol, or variable) |
| `inner_align` | `enum:lv_image_align` | Align and scale the image inside |
| `rotation` | `int` | Rotate the image (degrees) |
| `scale_x` | `int` | Scale horizontally (100 = original) |
| `scale_y` | `int` | Scale vertically (100 = original) |
| `pivot_x` | `int|%` | Set the pivot X for rotation/scaling |
| `pivot_y` | `int|%` | Set the pivot Y for rotation/scaling |
| `bind_src` | `subject:pointer` | Bind the image source to a pointer subject |

Last updated on
[GIF (lv_gif)Plays animated GIF images using LVGL's built-in decoder.](https://lvgl.io/docs/pro/built_in_widgets/lv_gif)[Image Button (lv_imagebutton)A button that draws per-state images instead of a styled rectangle.](https://lvgl.io/docs/pro/built_in_widgets/lv_imagebutton)
