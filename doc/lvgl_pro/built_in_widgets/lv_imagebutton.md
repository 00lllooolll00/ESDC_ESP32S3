---
url: https://lvgl.io/docs/pro/built_in_widgets/lv_imagebutton
title: Image Button (lv_imagebutton)
---

# Image Button (lv_imagebutton)

Source: <https://lvgl.io/docs/pro/built_in_widgets/lv_imagebutton>

[Built-in Widgets](https://lvgl.io/docs/pro/built_in_widgets)

# Image Button (lv_imagebutton)
A button that draws per-state images instead of a styled rectangle.
Copy MarkdownOpen

## Overview

The Image Button behaves like a [Button](https://lvgl.io/lv_button) but draws user-supplied images per state instead of a styled rectangle. Separate left, middle and right images can be set; the middle image is tiled to fill the width.

Learn more in the [LVGL Open documentation](https://lvgl.io/docs/open/widgets/imagebutton).

PreviewCXML
[lv_example_imagebutton.c](https://github.com/lvgl/lvgl_editor/blob/master/docs/examples/screens/lv_example_imagebutton.c)

## Enumerations

**`lv_imagebutton_state`** — Possible states of an image button

| Value | Description |
| --- | --- |
| `released` | Released |
| `pressed` | Pressed |
| `disabled` | Disabled |
| `checked_released` | Checked and released |
| `checked_pressed` | Checked and pressed |
| `checked_disabled` | Checked and disabled |

## Properties

### Parts

Style parts of the widget with local style (e.g. `style_bg_color-knob="0xff0000"`) or with style sheets (e.g. `<style name="style_knob" selector="knob">`). See [Styles](https://lvgl.io/syntax/styles) to learn more.

| Part | Description |
| --- | --- |
| `main` | Style the button: background, text and `image_recolor`/`image_recolor_opa` properties. The shown left/mid/right images change with the state (released, pressed, disabled, checked_*). |

Properties below are the widget's XML [`<api>`](https://lvgl.io/syntax/api); see [API](https://lvgl.io/syntax/api) for how properties, parameters and elements work.

| Property | Type | Description |
| --- | --- | --- |
| `state` | `enum:lv_imagebutton_state` | The current state of the imagebutton |

### `<lv_imagebutton-src_left>`

Set the left image source in a given state

*access `set`*

| Name | Kind | Type | Description |
| --- | --- | --- | --- |
| `state` | arg | `enum:lv_imagebutton_state` | Set the image in this state |
| `src` | arg | `image` | The image in the given state |

### `<lv_imagebutton-src_right>`

Set the right image source in a given state

*access `set`*

| Name | Kind | Type | Description |
| --- | --- | --- | --- |
| `state` | arg | `enum:lv_imagebutton_state` | Set the image in this state |
| `src` | arg | `image` | The image in the given state |

### `<lv_imagebutton-src_mid>`

Set the middle image source in a given state

*access `set`*

| Name | Kind | Type | Description |
| --- | --- | --- | --- |
| `state` | arg | `enum:lv_imagebutton_state` | Set the image in this state |
| `src` | arg | `image` | The image in the given state |

Last updated on
[Image (lv_image)Displays images from arrays or files, with recolor, scale and rotation.](https://lvgl.io/docs/pro/built_in_widgets/lv_image)[Keyboard (lv_keyboard)An on-screen virtual keyboard, a specialized Button Matrix.](https://lvgl.io/docs/pro/built_in_widgets/lv_keyboard)
