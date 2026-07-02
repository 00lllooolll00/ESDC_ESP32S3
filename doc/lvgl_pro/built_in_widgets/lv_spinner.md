---
url: https://lvgl.io/docs/pro/built_in_widgets/lv_spinner
title: Spinner (lv_spinner)
---

# Spinner (lv_spinner)

Source: <https://lvgl.io/docs/pro/built_in_widgets/lv_spinner>

[Built-in Widgets](https://lvgl.io/docs/pro/built_in_widgets)

# Spinner (lv_spinner)
A continuously rotating arc that indicates an activity is in progress.
Copy MarkdownOpen

## Overview

The Spinner shows a continuously rotating arc over a ring to indicate that an activity is in progress. Its spin duration and arc length are configurable.

Learn more in the [LVGL Open documentation](https://lvgl.io/docs/open/widgets/spinner).

PreviewCXML
[lv_example_spinner.c](https://github.com/lvgl/lvgl_editor/blob/master/docs/examples/screens/lv_example_spinner.c)

## Properties

### Parts

Style parts of the widget with local style (e.g. `style_bg_color-knob="0xff0000"`) or with style sheets (e.g. `<style name="style_knob" selector="knob">`). See [Styles](https://lvgl.io/syntax/styles) to learn more.

| Part | Description |
| --- | --- |
| `main` | The background arc (extends lv_arc): `arc_color`, `arc_width`, `arc_opa`, `arc_rounded`. |
| `indicator` | The spinning arc that animates around: the arc_* properties. |

Properties below are the widget's XML [`<api>`](https://lvgl.io/syntax/api); see [API](https://lvgl.io/syntax/api) for how properties, parameters and elements work.

| Property | Type | Description |
| --- | --- | --- |
| `anim_duration` | `int` | Set the animation time of the spinner. |
| `arc_sweep` | `int` | Set the animation arc length of the spinner. The animation is suited to values between 180 and 360. |

Last updated on
[Spinbox (lv_spinbox)A numeric editor built on a Text Area with fixed decimals and step.](https://lvgl.io/docs/pro/built_in_widgets/lv_spinbox)[Switch (lv_switch)A small slider that displays and toggles an on/off value.](https://lvgl.io/docs/pro/built_in_widgets/lv_switch)
