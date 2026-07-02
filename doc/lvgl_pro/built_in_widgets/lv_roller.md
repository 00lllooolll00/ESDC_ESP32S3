---
url: https://lvgl.io/docs/pro/built_in_widgets/lv_roller
title: Roller (lv_roller)
---

# Roller (lv_roller)

Source: <https://lvgl.io/docs/pro/built_in_widgets/lv_roller>

[Built-in Widgets](https://lvgl.io/docs/pro/built_in_widgets)

# Roller (lv_roller)
A wheel-like list where the centered item is the selection.
Copy MarkdownOpen

## Overview

The Roller lets the user pick one item from a list by scrolling it like a wheel. The centered item is the selected one and is usually styled to stand out from the rest.

Learn more in the [LVGL Open documentation](https://lvgl.io/docs/open/widgets/roller).

PreviewCXML
[lv_example_roller.c](https://github.com/lvgl/lvgl_editor/blob/master/docs/examples/screens/lv_example_roller.c)

## Enumerations

**`lv_roller_mode`** — How the roller scrolls

| Value | Description |
| --- | --- |
| `normal` | Stop at first and last option |
| `infinite` | Scroll endlessly in a loop |

## Properties

### Parts

Style parts of the widget with local style (e.g. `style_bg_color-knob="0xff0000"`) or with style sheets (e.g. `<style name="style_knob" selector="knob">`). See [Styles](https://lvgl.io/syntax/styles) to learn more.

| Part | Description |
| --- | --- |
| `main` | The background and option list: background and text properties. `text_line_space` sets the row spacing (and the height of the selected band); `anim_duration` sets the scroll animation time. |
| `selected` | The option in the middle band: text and background properties to highlight the selected row. Can use different font than `main` |

Properties below are the widget's XML [`<api>`](https://lvgl.io/syntax/api); see [API](https://lvgl.io/syntax/api) for how properties, parameters and elements work.

| Property | Type | Description |
| --- | --- | --- |
| `options` | `string` | Set the roller options |
| `options-mode` | `enum:lv_roller_mode` | Set the roller mode (reference as `options-mode`) (default: `normal`) |
| `selected` | `int` | Set the selected option |
| `selected-animated` | `bool` | Animate the selection (reference as `selected-animated`) (default: `false`) |
| `visible_row_count` | `int` | Show this many rows at once |
| `bind_value` | `subject` | Bind the selected option to a subject |

Last updated on
[QR Code (lv_qrcode)Generates and displays a QR-code bitmap from a string.](https://lvgl.io/docs/pro/built_in_widgets/lv_qrcode)[Scale (lv_scale)A linear or circular scale with ticks, labels, ranges and sections.](https://lvgl.io/docs/pro/built_in_widgets/lv_scale)
