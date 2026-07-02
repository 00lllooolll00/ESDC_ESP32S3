---
url: https://lvgl.io/docs/pro/built_in_widgets/lv_checkbox
title: Checkbox (lv_checkbox)
---

# Checkbox (lv_checkbox)

Source: <https://lvgl.io/docs/pro/built_in_widgets/lv_checkbox>

[Built-in Widgets](https://lvgl.io/docs/pro/built_in_widgets)

# Checkbox (lv_checkbox)
A tick box with a label that toggles when clicked.
Copy MarkdownOpen

## Overview

The Checkbox combines a tick box with a label. Clicking it toggles the checked state. The text is set directly on the widget and the tick box reflects the checked and disabled states.

Learn more in the [LVGL Open documentation](https://lvgl.io/docs/open/widgets/checkbox).

PreviewCXML
[lv_example_checkbox.c](https://github.com/lvgl/lvgl_editor/blob/master/docs/examples/screens/lv_example_checkbox.c)

## Properties

### Parts

Style parts of the widget with local style (e.g. `style_bg_color-knob="0xff0000"`) or with style sheets (e.g. `<style name="style_knob" selector="knob">`). See [Styles](https://lvgl.io/syntax/styles) to learn more.

| Part | Description |
| --- | --- |
| `main` | The label and its area: text properties; `pad_column` sets the gap between the tick box and the text. |
| `indicator` | The tick box: background, border, shadow and padding properties. `bg_image_src` sets the check icon. Use the 'checked' state (e.g. indicator|checked) for the checked look. |

Properties below are the widget's XML [`<api>`](https://lvgl.io/syntax/api); see [API](https://lvgl.io/syntax/api) for how properties, parameters and elements work.

| Property | Type | Description |
| --- | --- | --- |
| `text` | `string` | The label text shown next to the checkbox |

Last updated on
[Chart (lv_chart)Visualizes data series as lines, bars or scatter points with axes.](https://lvgl.io/docs/pro/built_in_widgets/lv_chart)[Drop-down List (lv_dropdown)Shows a selected value and opens a list to pick one option.](https://lvgl.io/docs/pro/built_in_widgets/lv_dropdown)
