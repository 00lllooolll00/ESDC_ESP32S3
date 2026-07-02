---
url: https://lvgl.io/docs/pro/built_in_widgets/lv_switch
title: Switch (lv_switch)
---

# Switch (lv_switch)

Source: <https://lvgl.io/docs/pro/built_in_widgets/lv_switch>

[Built-in Widgets](https://lvgl.io/docs/pro/built_in_widgets)

# Switch (lv_switch)
A small slider that displays and toggles an on/off value.
Copy MarkdownOpen

## Overview

The Switch looks like a small slider and displays — and optionally toggles — an on/off value. It is horizontal by default and becomes vertical when its width is smaller than its height.

Learn more in the [LVGL Open documentation](https://lvgl.io/docs/open/widgets/switch).

PreviewCXML
[lv_example_switch.c](https://github.com/lvgl/lvgl_editor/blob/master/docs/examples/screens/lv_example_switch.c)

## Enumerations

**`lv_switch_orientation`** — Switch orientation

| Value | Description |
| --- | --- |
| `auto` | Choose based on widget size |
| `horizontal` | Horizontal switch |
| `vertical` | Vertical switch |

## Properties

### Parts

Style parts of the widget with local style (e.g. `style_bg_color-knob="0xff0000"`) or with style sheets (e.g. `<style name="style_knob" selector="knob">`). See [Styles](https://lvgl.io/syntax/styles) to learn more.

| Part | Description |
| --- | --- |
| `main` | The background track: background properties. padding shrinks the indicator. |
| `indicator` | The fill that shows the on/off state: background properties. Use the 'checked' state (e.g. indicator|checked) for the ON color. |
| `knob` | The sliding handle: background, border, shadow and padding properties. |

Properties below are the widget's XML [`<api>`](https://lvgl.io/syntax/api); see [API](https://lvgl.io/syntax/api) for how properties, parameters and elements work.

| Property | Type | Description |
| --- | --- | --- |
| `orientation` | `enum:lv_switch_orientation` | Set switch orientation |

Last updated on
[Spinner (lv_spinner)A continuously rotating arc that indicates an activity is in progress.](https://lvgl.io/docs/pro/built_in_widgets/lv_spinner)[Table (lv_table)Lightweight rows, columns and cells of text drawn on the fly.](https://lvgl.io/docs/pro/built_in_widgets/lv_table)
