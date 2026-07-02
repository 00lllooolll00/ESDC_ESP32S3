---
url: https://lvgl.io/docs/pro/built_in_widgets/lv_slider
title: Slider (lv_slider)
---

# Slider (lv_slider)

Source: <https://lvgl.io/docs/pro/built_in_widgets/lv_slider>

[Built-in Widgets](https://lvgl.io/docs/pro/built_in_widgets)

# Slider (lv_slider)
A Bar with a draggable knob; supports range and symmetrical modes.
Copy MarkdownOpen

## Overview

The Slider is a [Bar](https://lvgl.io/lv_bar) with a draggable knob. It can be horizontal or vertical and supports normal, symmetrical and two-knob range modes.

Learn more in the [LVGL Open documentation](https://lvgl.io/docs/open/widgets/slider).

PreviewCXML
[lv_example_slider.c](https://github.com/lvgl/lvgl_editor/blob/master/docs/examples/screens/lv_example_slider.c)

## Enumerations

**`lv_slider_mode`** — How the slider behaves

| Value | Description |
| --- | --- |
| `normal` | One knob, grows left to right |
| `range` | Two knobs, select a range |
| `symmetrical` | Indicator grows from zero |

**`lv_slider_orientation`** — How the slider oriented

| Value | Description |
| --- | --- |
| `auto` | Decide based on width an height |
| `horizontal` | Always horizontally |
| `vertical` | Always vertical |

## Properties

### Parts

Style parts of the widget with local style (e.g. `style_bg_color-knob="0xff0000"`) or with style sheets (e.g. `<style name="style_knob" selector="knob">`). See [Styles](https://lvgl.io/syntax/styles) to learn more.

| Part | Description |
| --- | --- |
| `main` | The track/background: background properties. padding makes the indicator smaller in the respective direction. |
| `indicator` | The filled portion that shows the value: background properties. |
| `knob` | The draggable handle: background, border, shadow and padding properties. Use knob|pressed for drag feedback; range mode draws two knobs. |

Properties below are the widget's XML [`<api>`](https://lvgl.io/syntax/api); see [API](https://lvgl.io/syntax/api) for how properties, parameters and elements work.

| Property | Type | Description |
| --- | --- | --- |
| `min_value` | `int` | Set minimum value. If `min_value` > `max_value` fills from the right |
| `max_value` | `int` | Set maximum value. If `min_value` > `max_value` fills from the right |
| `value` | `int` | Set current value |
| `value-anim` | `bool` | Animate update (reference as `value-anim`) (default: `false`) |
| `start_value` | `int` | Set start value in range mode |
| `start_value-anim` | `bool` | Animate update (reference as `start_value-anim`) (default: `false`) |
| `mode` | `enum:lv_slider_mode` | Set slider mode |
| `bind_value` | `subject` | Bind value to subject |
| `orientation` | `enum:lv_slider_orientation` | Set the orientation to horizontal, vertical or auto |

Last updated on
[Scale (lv_scale)A linear or circular scale with ticks, labels, ranges and sections.](https://lvgl.io/docs/pro/built_in_widgets/lv_scale)[Span Group (lv_spangroup)Rich text built from spans, each with its own font, color and size.](https://lvgl.io/docs/pro/built_in_widgets/lv_spangroup)
