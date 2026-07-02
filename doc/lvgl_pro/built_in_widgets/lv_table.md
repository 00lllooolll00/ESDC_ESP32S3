---
url: https://lvgl.io/docs/pro/built_in_widgets/lv_table
title: Table (lv_table)
---

# Table (lv_table)

Source: <https://lvgl.io/docs/pro/built_in_widgets/lv_table>

[Built-in Widgets](https://lvgl.io/docs/pro/built_in_widgets)

# Table (lv_table)
Lightweight rows, columns and cells of text drawn on the fly.
Copy MarkdownOpen

## Overview

The Table displays text in rows, columns and cells. Only the cell text is stored and cells are drawn on the fly, so large tables stay lightweight. Cell selection via keyboard or encoder is supported.

Learn more in the [LVGL Open documentation](https://lvgl.io/docs/open/widgets/table).

PreviewCXML
[lv_example_table.c](https://github.com/lvgl/lvgl_editor/blob/master/docs/examples/screens/lv_example_table.c)

## Enumerations

**`lv_table_cell_ctrl`** — Cell control flags

| Value | Description |
| --- | --- |
| `merge_right` | Merge with right cell |
| `text_crop` | Crop text if too long |
| `custom_1` | User flag 1 |
| `custom_2` | User flag 2 |
| `custom_3` | User flag 3 |
| `custom_4` | User flag 4 |

## Properties

### Parts

Style parts of the widget with local style (e.g. `style_bg_color-knob="0xff0000"`) or with style sheets (e.g. `<style name="style_knob" selector="knob">`). See [Styles](https://lvgl.io/syntax/styles) to learn more.

| Part | Description |
| --- | --- |
| `main` | The table's outer frame and background: background, border and padding properties. |
| `items` | The cells: background and text properties, applied uniformly to all cells. Per-cell variations need cell ctrl flags or draw events. |
| `scrollbar` | The scrollbar shown when the table is larger than its area: `width` (thickness), background properties and padding. |

Properties below are the widget's XML [`<api>`](https://lvgl.io/syntax/api); see [API](https://lvgl.io/syntax/api) for how properties, parameters and elements work.

| Property | Type | Description |
| --- | --- | --- |
| `row_count` | `int` | Set row count |
| `column_count` | `int` | Set column count |

### `<lv_table-column>`

Configure a table column

*access `set`*

| Name | Kind | Type | Description |
| --- | --- | --- | --- |
| `column` | arg | `int` | Column index (0-based) |
| `width` | prop | `px` | Set column width |

### `<lv_table-cell>`

Configure a table cell

*access `set`*

| Name | Kind | Type | Description |
| --- | --- | --- | --- |
| `row` | arg | `int` | Row index (0-based) |
| `column` | arg | `int` | Column index (0-based) |
| `value` | prop | `string` | Set cell text |
| `ctrl` | prop | `enum:lv_table_cell_ctrl+` | Set cell flags |

Last updated on
[Switch (lv_switch)A small slider that displays and toggles an on/off value.](https://lvgl.io/docs/pro/built_in_widgets/lv_switch)[Tab View (lv_tabview)Organizes content into tabs with a configurable tab bar.](https://lvgl.io/docs/pro/built_in_widgets/lv_tabview)
