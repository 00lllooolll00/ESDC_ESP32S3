---
url: https://lvgl.io/docs/pro/built_in_widgets/lv_led
title: LED (lv_led)
---

# LED (lv_led)

Source: <https://lvgl.io/docs/pro/built_in_widgets/lv_led>

[Built-in Widgets](https://lvgl.io/docs/pro/built_in_widgets)

# LED (lv_led)
A rectangle or circle whose brightness simulates a glowing indicator.
Copy MarkdownOpen

## Overview

The LED is a rectangle or circle whose brightness can be adjusted to simulate a glowing indicator. A single color drives its background, border and shadow; lower brightness darkens it.

Learn more in the [LVGL Open documentation](https://lvgl.io/docs/open/widgets/led).

PreviewCXML
[lv_example_led.c](https://github.com/lvgl/lvgl_editor/blob/master/docs/examples/screens/lv_example_led.c)

## Properties

### Parts

Style parts of the widget with local style (e.g. `style_bg_color-knob="0xff0000"`) or with style sheets (e.g. `<style name="style_knob" selector="knob">`). See [Styles](https://lvgl.io/syntax/styles) to learn more.

| Part | Description |
| --- | --- |
| `main` | Style the LED: background, border, `radius` and shadow properties. `bg_color`, shadow and border colors are overridden by `color`, and `brightness` scales the overall look. |

Properties below are the widget's XML [`<api>`](https://lvgl.io/syntax/api); see [API](https://lvgl.io/syntax/api) for how properties, parameters and elements work.

| Property | Type | Description |
| --- | --- | --- |
| `color` | `color` | Set the color of the LED |
| `brightness` | `opa` | Set how dark or bright the LED should be |

Last updated on
[Label (lv_label)The widget used to display text, with wrapping and long-text modes.](https://lvgl.io/docs/pro/built_in_widgets/lv_label)[Line (lv_line)Draws straight line segments connecting an array of points.](https://lvgl.io/docs/pro/built_in_widgets/lv_line)
