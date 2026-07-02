---
url: https://lvgl.io/docs/pro/built_in_widgets/lv_dropdown
title: Drop-down List (lv_dropdown)
---

# Drop-down List (lv_dropdown)

Source: <https://lvgl.io/docs/pro/built_in_widgets/lv_dropdown>

[Built-in Widgets](https://lvgl.io/docs/pro/built_in_widgets)

# Drop-down List (lv_dropdown)
Shows a selected value and opens a list to pick one option.
Copy MarkdownOpen

## Overview

The Drop-down List shows a single selected value and, when activated, opens a list of options for the user to pick from. The list closes automatically once a new option is selected.

Learn more in the [LVGL Open documentation](https://lvgl.io/docs/open/widgets/dropdown).

PreviewCXML
[lv_example_dropdown.c](https://github.com/lvgl/lvgl_editor/blob/master/docs/examples/screens/lv_example_dropdown.c)

## Properties

### Parts

Style parts of the widget with local style (e.g. `style_bg_color-knob="0xff0000"`) or with style sheets (e.g. `<style name="style_knob" selector="knob">`). See [Styles](https://lvgl.io/syntax/styles) to learn more.

| Part | Description |
| --- | --- |
| `main` | The closed button: background and text properties. Gets the 'checked' state while the list is open. |
| `indicator` | The symbol shown next to the text (set via `symbol`): `image_recolor` and `image_recolor_opa`. |

Properties below are the widget's XML [`<api>`](https://lvgl.io/syntax/api); see [API](https://lvgl.io/syntax/api) for how properties, parameters and elements work.

| Property | Type | Description |
| --- | --- | --- |
| `text` | `string` | Set text displayed instead of the selected option label |
| `options` | `string` | Set options as a newline (`\n`) separated string |
| `selected` | `int` | Select an option by index (0-based) |
| `symbol` | `image` | Set a symbol shown next to the dropdown |
| `bind_value` | `subject` | Bind the selected option index to a subject |
| `dir` | `enum:lv_dir` | Tells in which direction the dropdown should be opened |

### `<lv_dropdown-list>`

The dropdown list object for styling or customization

*access `get` · returns `lv_obj`*

**Parts**

| Part | Description |
| --- | --- |
| `main` | The open list background: background, border and padding. `max_height` caps the open list size. |
| `scrollbar` | The list's scrollbar: `width` (thickness), background properties and padding on the respective side. |
| `selected` | The highlighted option: background and text properties. The 'checked' state styles the currently selected option and 'pressed' the one being pressed/keyed. |

*No attributes — created as an empty child.*

Last updated on
[Checkbox (lv_checkbox)A tick box with a label that toggles when clicked.](https://lvgl.io/docs/pro/built_in_widgets/lv_checkbox)[GIF (lv_gif)Plays animated GIF images using LVGL's built-in decoder.](https://lvgl.io/docs/pro/built_in_widgets/lv_gif)
