---
url: https://lvgl.io/docs/pro/built_in_widgets/lv_calendar
title: Calendar (lv_calendar)
---

# Calendar (lv_calendar)

Source: <https://lvgl.io/docs/pro/built_in_widgets/lv_calendar>

[Built-in Widgets](https://lvgl.io/docs/pro/built_in_widgets)

# Calendar (lv_calendar)
A month calendar that highlights today and user-defined dates.
Copy MarkdownOpen

## Overview

The Calendar shows the days of a month in a 7×7 grid with weekday names, highlights today, and can mark user-defined dates. The displayed month and the highlighted/selected dates are all configurable.

Learn more in the [LVGL Open documentation](https://lvgl.io/docs/open/widgets/calendar).

PreviewCXML
[lv_example_calendar.c](https://github.com/lvgl/lvgl_editor/blob/master/docs/examples/screens/lv_example_calendar.c)

## Properties

### Parts

Style parts of the widget with local style (e.g. `style_bg_color-knob="0xff0000"`) or with style sheets (e.g. `<style name="style_knob" selector="knob">`). See [Styles](https://lvgl.io/syntax/styles) to learn more.

| Part | Description |
| --- | --- |
| `main` | The calendar background: background and padding properties. |
| `items` | The day cells and weekday names (built on a button matrix): background and text properties. Weekday names and days outside the shown month (disabled) are drawn without background/border. Today's thicker border and the highlighted days' fill/border are forced to the theme's primary color in code, so they are not set via style properties. |

Properties below are the widget's XML [`<api>`](https://lvgl.io/syntax/api); see [API](https://lvgl.io/syntax/api) for how properties, parameters and elements work.

| Property | Type | Description |
| --- | --- | --- |
| `today_year` | `int` | Mark today’s year in the calendar |
| `today_month` | `int` | Mark today’s month in the calendar |
| `today_day` | `int` | Mark today’s day in the calendar |
| `shown_year` | `int` | Show this year in the calendar |
| `shown_month` | `int` | Show this month in the calendar |
| `chinese_mode` | `bool` | Show a Chinese calendar |

### `<lv_calendar-header_arrow>`

Add arrow buttons for navigating months

*access `add`*

*No attributes — created as an empty child.*

### `<lv_calendar-header_dropdown>`

Add a dropdown for selecting months or years

*access `add`*

*No attributes — created as an empty child.*

Last updated on
[Button Matrix (lv_buttonmatrix)Many buttons in rows and columns drawn virtually from a single text map.](https://lvgl.io/docs/pro/built_in_widgets/lv_buttonmatrix)[Canvas (lv_canvas)An Image extended with a user buffer the draw engine can render into.](https://lvgl.io/docs/pro/built_in_widgets/lv_canvas)
