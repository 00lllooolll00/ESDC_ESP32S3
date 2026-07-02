---
url: https://lvgl.io/docs/pro/built_in_widgets/lv_spinbox
title: Spinbox (lv_spinbox)
---

# Spinbox (lv_spinbox)

Source: <https://lvgl.io/docs/pro/built_in_widgets/lv_spinbox>

[Built-in Widgets](https://lvgl.io/docs/pro/built_in_widgets)

# Spinbox (lv_spinbox)
A numeric editor built on a Text Area with fixed decimals and step.
Copy MarkdownOpen

## Overview

The Spinbox is a numeric editor built on a [Text Area](https://lvgl.io/lv_textarea). It shows an integer with an optional fixed decimal point and digit count, and the value can be stepped within a range.

Learn more in the [LVGL Open documentation](https://lvgl.io/docs/open/widgets/spinbox).

PreviewCXML
[lv_example_spinbox.c](https://github.com/lvgl/lvgl_editor/blob/master/docs/examples/screens/lv_example_spinbox.c)

## Properties

### Parts

Style parts of the widget with local style (e.g. `style_bg_color-knob="0xff0000"`) or with style sheets (e.g. `<style name="style_knob" selector="knob">`). See [Styles](https://lvgl.io/syntax/styles) to learn more.

| Part | Description |
| --- | --- |
| `main` | The background and the number text (extends lv_textarea): background, padding and text properties. |
| `scrollbar` | The scrollbar shown when content overflows: `width` (thickness), background properties and padding. |
| `selected` | The selected text. Only `text_color` and `bg_color` are used. |
| `cursor` | The cursor over the edited digit: `bg_color`/`bg_opa` for a block cursor or a left border for a bar cursor. |

Properties below are the widget's XML [`<api>`](https://lvgl.io/syntax/api); see [API](https://lvgl.io/syntax/api) for how properties, parameters and elements work.

| Property | Type | Description |
| --- | --- | --- |
| `value` | `int` | Set the value as a raw integer. The displayed format depends on `digit_count` and `dec_point_pos` |
| `rollover` | `bool` | If enabled, stepping past the maximum jumps to the minimum and vice versa |
| `digit_count` | `int` | Set the number of digits, excluding the sign and the decimal separator |
| `dec_point_pos` | `int` | Set the number of digits before the decimal point. 0 means no decimal point is shown |
| `min_value` | `int` | Set the minimum value (inclusive) |
| `max_value` | `int` | Set the maximum value (inclusive) |
| `step` | `int` | Set the digit that changes on increment/decrement. Can be 1, 10, 100, etc. |
| `bind_value` | `subject` | Connect a subject to the spinbox's value |

Last updated on
[Span Group (lv_spangroup)Rich text built from spans, each with its own font, color and size.](https://lvgl.io/docs/pro/built_in_widgets/lv_spangroup)[Spinner (lv_spinner)A continuously rotating arc that indicates an activity is in progress.](https://lvgl.io/docs/pro/built_in_widgets/lv_spinner)
