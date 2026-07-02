---
url: https://lvgl.io/docs/pro/built_in_widgets/lv_scale
title: Scale (lv_scale)
---

# Scale (lv_scale)

Source: <https://lvgl.io/docs/pro/built_in_widgets/lv_scale>

[Built-in Widgets](https://lvgl.io/docs/pro/built_in_widgets)

# Scale (lv_scale)
A linear or circular scale with ticks, labels, ranges and sections.
Copy MarkdownOpen

## Overview

The Scale draws a linear or circular scale with a configurable range, tick counts and placement, optional labels, and styled sections for highlighting sub-ranges.

Learn more in the [LVGL Open documentation](https://lvgl.io/docs/open/widgets/scale).

PreviewCXML
[lv_example_scale.c](https://github.com/lvgl/lvgl_editor/blob/master/docs/examples/screens/lv_example_scale.c)

## Enumerations

**`lv_scale_mode`** — How the scale is drawn

| Value | Description |
| --- | --- |
| `horizontal_top` | Ticks on top |
| `horizontal_bottom` | Ticks on bottom |
| `vertical_left` | Ticks on left |
| `vertical_right` | Ticks on right |
| `round_inner` | Circular, ticks inside |
| `round_outer` | Circular, ticks outside |

## Properties

### Parts

Style parts of the widget with local style (e.g. `style_bg_color-knob="0xff0000"`) or with style sheets (e.g. `<style name="style_knob" selector="knob">`). See [Styles](https://lvgl.io/syntax/styles) to learn more.

| Part | Description |
| --- | --- |
| `main` | The main rail: on straight scales a line (`line_color`, `line_width`, `line_opa`, `line_rounded`); on round scales an arc (`arc_color`, `arc_width`, `arc_opa`, `arc_rounded`, `arc_image_src`). |
| `items` | The minor ticks (always drawn as lines): line properties; `length` sets the tick length and `radial_offset` shifts them on round scales. |
| `indicator` | The major ticks and their labels: line properties for the ticks (`length`, `radial_offset`) plus text properties for the labels; `pad_radial` sets the label gap. |

Properties below are the widget's XML [`<api>`](https://lvgl.io/syntax/api); see [API](https://lvgl.io/syntax/api) for how properties, parameters and elements work.

| Property | Type | Description |
| --- | --- | --- |
| `mode` | `enum:lv_scale_mode` | Set the scale mode |
| `total_tick_count` | `int` | Set number of ticks |
| `major_tick_every` | `int` | Set frequency of major ticks |
| `label_show` | `bool` | Show or hide labels |
| `min_value` | `int` | Set minimum value |
| `max_value` | `int` | Set maximum value |
| `angle_range` | `deg` | Set angular range (degrees) |
| `rotation` | `deg` | Rotate the scale (degrees) |
| `text_src` | `string[NULL]` | Set custom labels from text |
| `post_draw` | `bool` | Enable post-draw events |
| `draw_ticks_on_top` | `bool` | Draw ticks above other content |

### `<lv_scale-section>`

Add a styled section

*access `add` · returns `lv_scale_section`*

| Name | Kind | Type | Description |
| --- | --- | --- | --- |
| `min_value` | prop | `int` | Set section min |
| `max_value` | prop | `int` | Set section max |
| `style_main` | prop | `style` | Set main line style |
| `style_indicator` | prop | `style` | Set major line and text style |
| `style_items` | prop | `style` | Set minor ticks' style |
| `bind_min_value` | prop | `subject` | Bind section min to subject |
| `bind_max_value` | prop | `subject` | Bind section max to subject |

Last updated on
[Roller (lv_roller)A wheel-like list where the centered item is the selection.](https://lvgl.io/docs/pro/built_in_widgets/lv_roller)[Slider (lv_slider)A Bar with a draggable knob; supports range and symmetrical modes.](https://lvgl.io/docs/pro/built_in_widgets/lv_slider)
