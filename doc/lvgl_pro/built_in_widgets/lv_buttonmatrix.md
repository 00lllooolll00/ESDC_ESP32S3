---
url: https://lvgl.io/docs/pro/built_in_widgets/lv_buttonmatrix
title: Button Matrix (lv_buttonmatrix)
---

# Button Matrix (lv_buttonmatrix)

Source: <https://lvgl.io/docs/pro/built_in_widgets/lv_buttonmatrix>

[Built-in Widgets](https://lvgl.io/docs/pro/built_in_widgets)

# Button Matrix (lv_buttonmatrix)
Many buttons in rows and columns drawn virtually from a single text map.
Copy MarkdownOpen

## Overview

The Button Matrix displays many buttons arranged in rows and columns from a single text map. The buttons are drawn virtually rather than created as individual widgets, making it a very memory-efficient way to show a grid of buttons.

Learn more in the [LVGL Open documentation](https://lvgl.io/docs/open/widgets/buttonmatrix).

PreviewCXML
[lv_example_buttonmatrix.c](https://github.com/lvgl/lvgl_editor/blob/master/docs/examples/screens/lv_example_buttonmatrix.c)

## Enumerations

**`lv_buttonmatrix_ctrl`** — Control flags you can apply to individual buttons *(values can be combined)*

| Value | Description |
| --- | --- |
| `none` | Make the button use the default state with no special control |
| `width_1` | Make the button take 1 unit of relative width in its row |
| `width_2` | Make the button take 2 units of relative width in its row |
| `width_3` | Make the button take 3 units of relative width in its row |
| `width_4` | Make the button take 4 units of relative width in its row |
| `width_5` | Make the button take 5 units of relative width in its row |
| `width_6` | Make the button take 6 units of relative width in its row |
| `width_7` | Make the button take 7 units of relative width in its row |
| `width_8` | Make the button take 8 units of relative width in its row |
| `width_9` | Make the button take 9 units of relative width in its row |
| `width_10` | Make the button take 10 units of relative width in its row |
| `width_11` | Make the button take 11 units of relative width in its row |
| `width_12` | Make the button take 12 units of relative width in its row |
| `width_13` | Make the button take 13 units of relative width in its row |
| `width_14` | Make the button take 14 units of relative width in its row |
| `width_15` | Make the button take 15 units of relative width in its row |
| `hidden` | Make the button hidden |
| `no_repeat` | Disable long-press repeat on the button |
| `disabled` | Make the button disabled (not clickable) |
| `checkable` | Make the button toggleable when clicked |
| `checked` | Keep the button in the toggled (checked) state |
| `click_trig` | Trigger LV_EVENT_VALUE_CHANGED on CLICK instead of PRESS |
| `popover` | Show a popover when pressing the button |
| `recolor` | Enable text recoloring with inline #color codes |
| `reserved_1` | Reserved flag for future use |
| `reserved_2` | Reserved flag for future use |
| `custom_1` | Custom flag free to use by the user |
| `custom_2` | Custom flag free to use by the user |

## Properties

### Parts

Style parts of the widget with local style (e.g. `style_bg_color-knob="0xff0000"`) or with style sheets (e.g. `<style name="style_knob" selector="knob">`). See [Styles](https://lvgl.io/syntax/styles) to learn more.

| Part | Description |
| --- | --- |
| `main` | The background behind the buttons: background properties. `pad_row` and `pad_column` set the gaps between buttons. |
| `items` | The buttons: text and background properties (transforms are not applied). Combine with the 'checked', 'pressed', 'focused' and 'disabled' states, e.g. items|checked. |

Properties below are the widget's XML [`<api>`](https://lvgl.io/syntax/api); see [API](https://lvgl.io/syntax/api) for how properties, parameters and elements work.

| Property | Type | Description |
| --- | --- | --- |
| `map` | `string[NULL]` | Set the layout map of the button matrix using text strings and `\n` for line breaks (e.g. 'a' 'b' 'c') |
| `ctrl_map` | `enum:lv_buttonmatrix_ctrl[]+` | Set the control flags for each button in the map |
| `one_checked` | `bool` | Force exactly one button to stay checked at all times |
| `selected_button` | `int` | Select a button by index (new lines do not count) |

Last updated on
[Button (lv_button)A clickable Base Widget with defaults tuned for its role.](https://lvgl.io/docs/pro/built_in_widgets/lv_button)[Calendar (lv_calendar)A month calendar that highlights today and user-defined dates.](https://lvgl.io/docs/pro/built_in_widgets/lv_calendar)
