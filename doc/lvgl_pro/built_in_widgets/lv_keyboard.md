---
url: https://lvgl.io/docs/pro/built_in_widgets/lv_keyboard
title: Keyboard (lv_keyboard)
---

# Keyboard (lv_keyboard)

Source: <https://lvgl.io/docs/pro/built_in_widgets/lv_keyboard>

[Built-in Widgets](https://lvgl.io/docs/pro/built_in_widgets)

# Keyboard (lv_keyboard)
An on-screen virtual keyboard, a specialized Button Matrix.
Copy MarkdownOpen

## Overview

The Keyboard is a specialized [Button Matrix](https://lvgl.io/lv_buttonmatrix) with predefined keymaps that provides an on-screen virtual keyboard. It can be attached to a [Text Area](https://lvgl.io/lv_textarea) so key presses edit its text.

Learn more in the [LVGL Open documentation](https://lvgl.io/docs/open/widgets/keyboard).

PreviewCXML
[lv_example_keyboard.c](https://github.com/lvgl/lvgl_editor/blob/master/docs/examples/screens/lv_example_keyboard.c)

## Enumerations

**`lv_keyboard_mode`** — Keyboard layout modes

| Value | Description |
| --- | --- |
| `text_upper` | Show uppercase letters |
| `text_lower` | Show lowercase letters |
| `text_arabic` | Show Arabic characters |
| `special` | Show special characters and symbols |
| `number` | Show numeric keys |
| `user_1` | User-defined layout 1 |
| `user_2` | User-defined layout 2 |
| `user_3` | User-defined layout 3 |
| `user_4` | User-defined layout 4 |

## Properties

### Parts

Style parts of the widget with local style (e.g. `style_bg_color-knob="0xff0000"`) or with style sheets (e.g. `<style name="style_knob" selector="knob">`). See [Styles](https://lvgl.io/syntax/styles) to learn more.

| Part | Description |
| --- | --- |
| `main` | The background behind the keys (extends lv_buttonmatrix): background properties. `pad_row` and `pad_column` set the gaps between keys. |
| `items` | The keys: text and background properties. Combine with the 'checked', 'pressed', 'focused' and 'disabled' states, e.g. items|pressed. |

Properties below are the widget's XML [`<api>`](https://lvgl.io/syntax/api); see [API](https://lvgl.io/syntax/api) for how properties, parameters and elements work.

| Property | Type | Description |
| --- | --- | --- |
| `mode` | `enum:lv_keyboard_mode` | Set the keyboard layout |
| `popovers` | `bool` | Show enlarged key previews on press |
| `textarea` | `lv_obj` | Connect a textare to the keyboard |

Last updated on
[Image Button (lv_imagebutton)A button that draws per-state images instead of a styled rectangle.](https://lvgl.io/docs/pro/built_in_widgets/lv_imagebutton)[Label (lv_label)The widget used to display text, with wrapping and long-text modes.](https://lvgl.io/docs/pro/built_in_widgets/lv_label)
