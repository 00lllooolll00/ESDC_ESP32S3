---
url: https://lvgl.io/docs/pro/built_in_widgets/lv_arc
title: Arc (lv_arc)
---

# Arc (lv_arc)

Source: <https://lvgl.io/docs/pro/built_in_widgets/lv_arc>

[Built-in Widgets](https://lvgl.io/docs/pro/built_in_widgets)

# Arc (lv_arc)
A background ring with a touch-adjustable foreground indicator arc.
Copy MarkdownOpen

## Overview

The Arc draws a background ring and a foreground indicator arc. The indicator can be set programmatically or dragged by touch, and its value is interpreted within a configurable min/max range and start/end angles.

Learn more in the [LVGL Open documentation](https://lvgl.io/docs/open/widgets/arc).

PreviewCXML
[lv_example_arc.c](https://github.com/lvgl/lvgl_editor/blob/master/docs/examples/screens/lv_example_arc.c)

## Enumerations

**`lv_arc_mode`** — How the arc’s indicator grows

| Value | Description |
| --- | --- |
| `normal` | Make the indicator grow clockwise |
| `symmetrical` | Make the indicator grow symmetrically left and right from the midpoint |
| `reverse` | Make the indicator grow counterclockwise |

## Properties

### Parts

Style parts of the widget with local style (e.g. `style_bg_color-knob="0xff0000"`) or with style sheets (e.g. `<style name="style_knob" selector="knob">`). See [Styles](https://lvgl.io/syntax/styles) to learn more.

| Part | Description |
| --- | --- |
| `main` | The background arc: `arc_color`, `arc_width`, `arc_opa`, `arc_rounded` and `arc_image_src`. Background properties draw a box behind it. |
| `indicator` | The value arc drawn over the background: the arc_* properties. Its padding is relative to the background arc. |
| `knob` | The handle at the end of the indicator: background, border, shadow and padding properties (padding enlarges it). |

Properties below are the widget's XML [`<api>`](https://lvgl.io/syntax/api); see [API](https://lvgl.io/syntax/api) for how properties, parameters and elements work.

| Property | Type | Description |
| --- | --- | --- |
| `start_angle` | `int` | Set the start angle of the indicator |
| `end_angle` | `int` | Set the end angle of the indicator |
| `bg_start_angle` | `int` | Set the start angle of the background arc |
| `bg_end_angle` | `int` | Set the end angle of the background arc |
| `rotation` | `int` | Rotate the whole arc by a given angle |
| `min_value` | `int` | Set the minimum value of the arc |
| `max_value` | `int` | Set the maximum value of the arc |
| `value` | `int` | Set the current value of the arc (between minimum and maximum) |
| `mode` | `enum:lv_arc_mode` | Select how the indicator grows |
| `change_rate` | `int` | How fast the arc should jump to the clicked value (deg/sec) |
| `bind_value` | `subject` | Bind the arc’s value to a subject |

Last updated on
[Base Widget (lv_obj)The Base Widget every other widget inherits from, and the generic container used to compose layouts in XML.](https://lvgl.io/docs/pro/built_in_widgets/lv_obj)[Bar (lv_bar)A background with an indicator whose length represents a value within a range.](https://lvgl.io/docs/pro/built_in_widgets/lv_bar)
