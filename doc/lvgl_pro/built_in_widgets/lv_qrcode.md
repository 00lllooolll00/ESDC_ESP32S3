---
url: https://lvgl.io/docs/pro/built_in_widgets/lv_qrcode
title: QR Code (lv_qrcode)
---

# QR Code (lv_qrcode)

Source: <https://lvgl.io/docs/pro/built_in_widgets/lv_qrcode>

[Built-in Widgets](https://lvgl.io/docs/pro/built_in_widgets)

# QR Code (lv_qrcode)
Generates and displays a QR-code bitmap from a string.
Copy MarkdownOpen

## Overview

The QR Code widget generates and displays a QR-code bitmap from a string using LVGL's QR-code library. Its size, colors and the encoded text are configurable.

Learn more in the [LVGL Open documentation](https://lvgl.io/docs/open/libs/qrcode).

PreviewCXML
[lv_example_qrcode.c](https://github.com/lvgl/lvgl_editor/blob/master/docs/examples/screens/lv_example_qrcode.c)

## Properties

### Parts

Style parts of the widget with local style (e.g. `style_bg_color-knob="0xff0000"`) or with style sheets (e.g. `<style name="style_knob" selector="knob">`). See [Styles](https://lvgl.io/syntax/styles) to learn more.

| Part | Description |
| --- | --- |
| `main` | Style the area behind the QR code: background, border and padding. The module colors come from dark_color/light_color, not from style properties. |

Properties below are the widget's XML [`<api>`](https://lvgl.io/syntax/api); see [API](https://lvgl.io/syntax/api) for how properties, parameters and elements work.

| Property | Type | Description |
| --- | --- | --- |
| `size` | `int` | Set the QR code size in pixels (instead of width/height) |
| `dark_color` | `color` | Set the foreground color |
| `light_color` | `color` | Set the background color |
| `data` | `string` | Set the encoded UTF-8 data |
| `quiet_zone` | `bool` | Add margin around the QR code |

Last updated on
[Line (lv_line)Draws straight line segments connecting an array of points.](https://lvgl.io/docs/pro/built_in_widgets/lv_line)[Roller (lv_roller)A wheel-like list where the centered item is the selection.](https://lvgl.io/docs/pro/built_in_widgets/lv_roller)
