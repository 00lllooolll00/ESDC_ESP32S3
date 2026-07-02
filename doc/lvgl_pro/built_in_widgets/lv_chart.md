---
url: https://lvgl.io/docs/pro/built_in_widgets/lv_chart
title: Chart (lv_chart)
---

# Chart (lv_chart)

Source: <https://lvgl.io/docs/pro/built_in_widgets/lv_chart>

[Built-in Widgets](https://lvgl.io/docs/pro/built_in_widgets)

# Chart (lv_chart)
Visualizes data series as lines, bars or scatter points with axes.
Copy MarkdownOpen

## Overview

The Chart visualizes one or more data series as lines, bars or scatter points. It supports division lines, up to four axes, a configurable point count, and runtime add/remove of series and points.

Learn more in the [LVGL Open documentation](https://lvgl.io/docs/open/widgets/chart).

PreviewCXML
[lv_example_chart.c](https://github.com/lvgl/lvgl_editor/blob/master/docs/examples/screens/lv_example_chart.c)

## Enumerations

**`lv_chart_type`** — How the chart draws its data series

| Value | Description |
| --- | --- |
| `none` | Draw nothing |
| `line` | Draw data points as connected lines |
| `bar` | Draw data points as bars |
| `stacked` | Draw series as stacked bars on top of each other |
| `scatter` | Draw data points as unconnected dots |

**`lv_chart_update_mode`** — How the chart handles new data points

| Value | Description |
| --- | --- |
| `shift` | Shift old data to the left when new points are added |
| `circular` | Overwrite the oldest data in a circular buffer style |

**`lv_chart_axis`** — Which axis a series belongs to

| Value | Description |
| --- | --- |
| `primary_x` | Use the primary X axis |
| `primary_y` | Use the primary Y axis |
| `secondary_x` | Use the secondary X axis |
| `secondary_y` | Use the secondary Y axis |

## Properties

### Parts

Style parts of the widget with local style (e.g. `style_bg_color-knob="0xff0000"`) or with style sheets (e.g. `<style name="style_knob" selector="knob">`). See [Styles](https://lvgl.io/syntax/styles) to learn more.

| Part | Description |
| --- | --- |
| `main` | The background and the division lines: background properties, plus the line properties (`line_color`/width/dash) for the division lines. `pad_column` sets the space between `BAR` column groups. |
| `items` | The data series body. For LINE charts the connecting lines use the line properties (`line_color`, `line_width`, line_dash, `line_rounded`). For BAR charts the bars use the background properties (`bg_color`, `radius`, border). `pad_column` sets the gap between bars on the same X. |
| `indicator` | The point markers drawn at each value on LINE and SCATTER charts: `width`/`height` set the marker size, `bg_color` its fill and `radius` its roundness; border, outline, and shadow properties also apply. |
| `cursor` | The cursors: the line properties draw the crosshair lines and the marker at the cursor point uses `width` together with `bg_color`/`radius`. |
| `scrollbar` | The scrollbar shown when the chart is zoomed/scrolled: `width` (thickness), background properties and padding. |

Properties below are the widget's XML [`<api>`](https://lvgl.io/syntax/api); see [API](https://lvgl.io/syntax/api) for how properties, parameters and elements work.

| Property | Type | Description |
| --- | --- | --- |
| `type` | `enum:lv_chart_type` | Choose how the chart should draw its data |
| `point_count` | `int` | Set how many data points each series stores |
| `update_mode` | `enum:lv_chart_update_mode` | Choose how the chart updates when adding new points |
| `hor_div_line_count` | `int` | Show this many horizontal division lines |
| `ver_div_line_count` | `int` | Show this many vertical division lines |

### `<lv_chart-series>`

Add a data series to the chart

*access `add` · returns `lv_chart_series`*

| Name | Kind | Type | Description |
| --- | --- | --- | --- |
| `color` | arg | `color` | Set the color of the series |
| `axis` | arg | `enum:lv_chart_axis+` | Choose which axis the series belongs to (default: `primary_y`) |
| `values` | prop | `int[count]` | Provide the data values of the series |

### `<lv_chart-cursor>`

Add a cursor indicator to the chart

*access `add` · returns `lv_chart_cursor`*

| Name | Kind | Type | Description |
| --- | --- | --- | --- |
| `color` | arg | `color` | Set the color of the cursor |
| `dir` | arg | `enum:lv_dir` | Choose the direction of the cursor |
| `pos_x` | prop | `int` | Set the X position of the cursor |
| `pos_y` | prop | `int` | Set the Y position of the cursor |

### `<lv_chart-axis>`

Configure the range of a chart axis

*access `set`*

| Name | Kind | Type | Description |
| --- | --- | --- | --- |
| `axis` | arg | `enum:lv_chart_axis` | Choose which axis to configure |
| `min_value` | prop | `int` | Set the minimum value of the axis |
| `max_value` | prop | `int` | Set the maximum value of the axis |

Last updated on
[Canvas (lv_canvas)An Image extended with a user buffer the draw engine can render into.](https://lvgl.io/docs/pro/built_in_widgets/lv_canvas)[Checkbox (lv_checkbox)A tick box with a label that toggles when clicked.](https://lvgl.io/docs/pro/built_in_widgets/lv_checkbox)
