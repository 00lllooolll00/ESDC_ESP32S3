---
url: https://lvgl.io/docs/pro/built_in_widgets/lv_textarea
title: Text Area (lv_textarea)
---

# Text Area (lv_textarea)

Source: <https://lvgl.io/docs/pro/built_in_widgets/lv_textarea>

[Built-in Widgets](https://lvgl.io/docs/pro/built_in_widgets)

# Text Area (lv_textarea)
An editable text input with a cursor; supports one-line and password modes.
Copy MarkdownOpen

## Overview

The Text Area is an editable text input with a cursor. It wraps long lines, can scroll, and supports one-line and password modes plus character and accepted-list filtering.

Learn more in the [LVGL Open documentation](https://lvgl.io/docs/open/widgets/textarea).

PreviewCXML
[lv_example_textarea.c](https://github.com/lvgl/lvgl_editor/blob/master/docs/examples/screens/lv_example_textarea.c)

## Properties

### Parts

Style parts of the widget with local style (e.g. `style_bg_color-knob="0xff0000"`) or with style sheets (e.g. `<style name="style_knob" selector="knob">`). See [Styles](https://lvgl.io/syntax/styles) to learn more.

| Part | Description |
| --- | --- |
| `main` | The background and the typed text: background, padding and text properties. |
| `scrollbar` | The scrollbar shown when the text is taller than the area: `width` (thickness), background properties and padding on the respective side. |
| `selected` | The selected text. Only `text_color` and `bg_color` are used. |
| `cursor` | The insertion cursor: set `bg_color`/`bg_opa` for a block cursor or a left border (`border_side`='left') for a bar cursor. `anim_duration` controls blinking. |
| `textarea_placeholder` | The placeholder text shown when empty: text properties (e.g. `text_color`, `text_font`). |

Properties below are the widget's XML [`<api>`](https://lvgl.io/syntax/api); see [API](https://lvgl.io/syntax/api) for how properties, parameters and elements work.

| Property | Type | Description |
| --- | --- | --- |
| `text` | `string` | Set textarea text |
| `placeholder_text` | `string` | Set placeholder text |
| `one_line` | `bool` | Enable single-line mode |
| `password_mode` | `bool` | Enable password masking |
| `password_show_time` | `int` | Set delay to show typed chars (ms) |
| `text_selection` | `bool` | Enable text selection |
| `cursor_pos` | `int` | Set cursor position (0-based) |

Last updated on
[Tab View (lv_tabview)Organizes content into tabs with a configurable tab bar.](https://lvgl.io/docs/pro/built_in_widgets/lv_tabview)[IntegrationGuides for integrating LVGL Pro Editor content into your projects using C code, XML files, and various platforms.](https://lvgl.io/docs/pro/integration)
