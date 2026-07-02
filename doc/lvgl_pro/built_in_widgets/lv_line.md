---
url: https://lvgl.io/docs/pro/built_in_widgets/lv_line
title: Line (lv_line)
---

# Line (lv_line)

Source: <https://lvgl.io/docs/pro/built_in_widgets/lv_line>

[Built-in Widgets](https://lvgl.io/docs/pro/built_in_widgets)

# Line (lv_line)
Draws straight line segments connecting an array of points.
Copy MarkdownOpen

## Overview

The Line widget draws straight line segments connecting an array of points. It is commonly used for simple vector drawing and for connecting other widgets visually.

Learn more in the [LVGL Open documentation](https://lvgl.io/docs/open/widgets/line).

## Properties

### Parts

Style parts of the widget with local style (e.g. `style_bg_color-knob="0xff0000"`) or with style sheets (e.g. `<style name="style_knob" selector="knob">`). See [Styles](https://lvgl.io/syntax/styles) to learn more.

| Part | Description |
| --- | --- |
| `main` | Style the line: `line_width`, `line_color`, `line_opa`, `line_rounded` and `line_dash_width`/`line_dash_gap`. Background can be added too. |

Properties below are the widget's XML [`<api>`](https://lvgl.io/syntax/api); see [API](https://lvgl.io/syntax/api) for how properties, parameters and elements work.

| Property | Type | Description |
| --- | --- | --- |
| `y_invert` | `bool` | If true the y=0 of points will be at the bottom of the widget |
| `points` | `precise_points[count]` | Set the points of the line. E.g. (10 20) (30 40) |

Last updated on
[LED (lv_led)A rectangle or circle whose brightness simulates a glowing indicator.](https://lvgl.io/docs/pro/built_in_widgets/lv_led)[QR Code (lv_qrcode)Generates and displays a QR-code bitmap from a string.](https://lvgl.io/docs/pro/built_in_widgets/lv_qrcode)
