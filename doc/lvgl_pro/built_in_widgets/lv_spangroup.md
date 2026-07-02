---
url: https://lvgl.io/docs/pro/built_in_widgets/lv_spangroup
title: Span Group (lv_spangroup)
---

# Span Group (lv_spangroup)

Source: <https://lvgl.io/docs/pro/built_in_widgets/lv_spangroup>

[Built-in Widgets](https://lvgl.io/docs/pro/built_in_widgets)

# Span Group (lv_spangroup)
Rich text built from spans, each with its own font, color and size.
Copy MarkdownOpen

## Overview

The Span Group displays rich text built from multiple spans, each with its own text, font, color and size. Spans can be added or removed at runtime.

Learn more in the [LVGL Open documentation](https://lvgl.io/docs/open/widgets/spangroup).

PreviewCXML
[lv_example_spangroup.c](https://github.com/lvgl/lvgl_editor/blob/master/docs/examples/screens/lv_example_spangroup.c)

## Enumerations

**`lv_span_overflow`** — How to handle overflowing text

| Value | Description |
| --- | --- |
| `clip` | Clip at boundary |
| `ellipsis` | Replace overflowing words with ... |

## Properties

### Parts

Style parts of the widget with local style (e.g. `style_bg_color-knob="0xff0000"`) or with style sheets (e.g. `<style name="style_knob" selector="knob">`). See [Styles](https://lvgl.io/syntax/styles) to learn more.

| Part | Description |
| --- | --- |
| `main` | Style the text block and its background: text and background properties. Style individual spans via each span's own `style`. |

Properties below are the widget's XML [`<api>`](https://lvgl.io/syntax/api); see [API](https://lvgl.io/syntax/api) for how properties, parameters and elements work.

| Property | Type | Description |
| --- | --- | --- |
| `overflow` | `enum:lv_span_overflow` | Set overflow mode |
| `max_lines` | `int` | Set max lines |
| `indent` | `int` | Set first line indent (px) |

### `<lv_spangroup-span>`

Add a styled text span

*access `add` · returns `lv_span`*

| Name | Kind | Type | Description |
| --- | --- | --- | --- |
| `text` | prop | `string` | Set span text |
| `style` | prop | `style` | Set span style |
| `bind_text` | prop | `subject` | Bind text to a subject |
| `bind_text-fmt` | prop | `string` | Format string (printf style) (default: `NULL`) |

Last updated on
[Slider (lv_slider)A Bar with a draggable knob; supports range and symmetrical modes.](https://lvgl.io/docs/pro/built_in_widgets/lv_slider)[Spinbox (lv_spinbox)A numeric editor built on a Text Area with fixed decimals and step.](https://lvgl.io/docs/pro/built_in_widgets/lv_spinbox)
