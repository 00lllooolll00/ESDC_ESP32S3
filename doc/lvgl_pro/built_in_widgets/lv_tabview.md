---
url: https://lvgl.io/docs/pro/built_in_widgets/lv_tabview
title: Tab View (lv_tabview)
---

# Tab View (lv_tabview)

Source: <https://lvgl.io/docs/pro/built_in_widgets/lv_tabview>

[Built-in Widgets](https://lvgl.io/docs/pro/built_in_widgets)

# Tab View (lv_tabview)
Organizes content into tabs with a configurable tab bar.
Copy MarkdownOpen

## Overview

The Tab View organizes content into tabs with a tab bar. Each tab is a container you can fill with any widgets; the tab bar's position and size are configurable.

Learn more in the [LVGL Open documentation](https://lvgl.io/docs/open/widgets/tabview).

PreviewCXML
[lv_example_tabview.c](https://github.com/lvgl/lvgl_editor/blob/master/docs/examples/screens/lv_example_tabview.c)

## Properties

### Parts

Style parts of the widget with local style (e.g. `style_bg_color-knob="0xff0000"`) or with style sheets (e.g. `<style name="style_knob" selector="knob">`). See [Styles](https://lvgl.io/syntax/styles) to learn more.

| Part | Description |
| --- | --- |
| `main` | The tab view body (the content container background): background and padding properties. The tab bar is a separate object styled via the `tab_bar` element, and each tab button (an lv_button) via the `tab_button` element. |

Properties below are the widget's XML [`<api>`](https://lvgl.io/syntax/api); see [API](https://lvgl.io/syntax/api) for how properties, parameters and elements work.

| Property | Type | Description |
| --- | --- | --- |
| `active` | `int` | Set active tab index (0-based) |
| `active-anim` | `bool` | Animate update (reference as `active-anim`) (default: `false`) |
| `tab_bar_position` | `enum:lv_dir` | Set tab bar position (top, bottom, left, right) |

### `<lv_tabview-tab_bar>`

Get the tab bar object

*access `get` · returns `lv_obj`*

**Parts**

| Part | Description |
| --- | --- |
| `main` | The tab bar container holding the tab buttons: background and padding properties. |
| `scrollbar` | The tab bar's scrollbar shown when the buttons overflow: `width` (thickness), background properties and padding. |

*No attributes — created as an empty child.*

### `<lv_tabview-tab>`

Add a tab with content

*access `add` · returns `lv_obj`*

**Parts**

| Part | Description |
| --- | --- |
| `main` | The tab content page: background and padding properties. |
| `scrollbar` | The page's scrollbar shown when its content overflows: `width` (thickness), background properties and padding. |

| Name | Kind | Type | Description |
| --- | --- | --- | --- |
| `text` | arg | `string` | Set tab button text |

### `<lv_tabview-tab_button>`

Get a specific tab button

*access `get` · returns `lv_obj`*

**Parts**

| Part | Description |
| --- | --- |
| `main` | The tab button (an lv_button): background, border and text properties. Use the 'checked' state for the active tab and 'pressed'/'focused' for feedback. |

| Name | Kind | Type | Description |
| --- | --- | --- | --- |
| `index` | arg | `int` | Tab index (0-based) |

Last updated on
[Table (lv_table)Lightweight rows, columns and cells of text drawn on the fly.](https://lvgl.io/docs/pro/built_in_widgets/lv_table)[Text Area (lv_textarea)An editable text input with a cursor; supports one-line and password modes.](https://lvgl.io/docs/pro/built_in_widgets/lv_textarea)
