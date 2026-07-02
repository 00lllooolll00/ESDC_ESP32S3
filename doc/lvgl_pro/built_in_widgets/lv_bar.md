---
url: https://lvgl.io/docs/pro/built_in_widgets/lv_bar
title: Bar (lv_bar)
---

# Bar (lv_bar)

Source: <https://lvgl.io/docs/pro/built_in_widgets/lv_bar>

[Built-in Widgets](https://lvgl.io/docs/pro/built_in_widgets)

# Bar (lv_bar)
A background with an indicator whose length represents a value within a range.
Copy MarkdownOpen

## Overview

The Bar has a background and an indicator whose length represents the current value within a range. Both the start and end values can be set, so the indicator can grow from any point, and it animates between values.

Learn more in the [LVGL Open documentation](https://lvgl.io/docs/open/widgets/bar).

PreviewCXML
[lv_example_bar.c](https://github.com/lvgl/lvgl_editor/blob/master/docs/examples/screens/lv_example_bar.c)

## Enumerations

**`lv_bar_mode`** — How the bar grows with values

| Value | Description |
| --- | --- |
| `normal` | Make the bar grow from minimum toward maximum |
| `symmetrical` | Make the bar grow symmetrically from the midpoint |
| `range` | Make the bar show values between start and end points |

**`lv_bar_orientation`** — How the bar is oriented

| Value | Description |
| --- | --- |
| `auto` | Choose orientation automatically based on widget size |
| `horizontal` | Make the bar fill left to right |
| `vertical` | Make the bar fill bottom to top |

## Properties

### Parts

Style parts of the widget with local style (e.g. `style_bg_color-knob="0xff0000"`) or with style sheets (e.g. `<style name="style_knob" selector="knob">`). See [Styles](https://lvgl.io/syntax/styles) to learn more.

| Part | Description |
| --- | --- |
| `main` | The bar's background track: background properties. padding shrinks the indicator in the respective direction. |
| `indicator` | The filled portion that shows the value: background properties. |

Properties below are the widget's XML [`<api>`](https://lvgl.io/syntax/api); see [API](https://lvgl.io/syntax/api) for how properties, parameters and elements work.

| Property | Type | Description |
| --- | --- | --- |
| `min_value` | `int` | Set the minimum value of the bar. . If `min_value` > `max_value` fills from the right. |
| `max_value` | `int` | Set the maximum value of the bar. If `min_value` > `max_value` fills from the right |
| `value` | `int` | Set the current value of the bar |
| `value-animated` | `bool` | Animate the bar when changing its value. Reference as `value-animated` (default: `false`) |
| `start_value` | `int` | Set the start value in range mode |
| `start_value-animated` | `bool` | Animate the bar when changing its start value. Reference as `start_value-animated` (default: `false`) |
| `mode` | `enum:lv_bar_mode` | Select how the bar grows |
| `orientation` | `enum:lv_bar_orientation` | Select the bar’s orientation |
| `bind_value` | `subject` | Bind the bar’s value to a subject |

Last updated on
[Arc (lv_arc)A background ring with a touch-adjustable foreground indicator arc.](https://lvgl.io/docs/pro/built_in_widgets/lv_arc)[Button (lv_button)A clickable Base Widget with defaults tuned for its role.](https://lvgl.io/docs/pro/built_in_widgets/lv_button)
