---
url: https://lvgl.io/docs/pro/built_in_widgets/lv_canvas
title: Canvas (lv_canvas)
---

# Canvas (lv_canvas)

Source: <https://lvgl.io/docs/pro/built_in_widgets/lv_canvas>

[Built-in Widgets](https://lvgl.io/docs/pro/built_in_widgets)

# Canvas (lv_canvas)
An Image extended with a user buffer the draw engine can render into.
Copy MarkdownOpen

## Overview

The Canvas extends the [Image](https://lvgl.io/lv_image) widget with a user-provided buffer that LVGL's draw engine can render into. Rectangles, text, images, lines and arcs can all be drawn onto the buffer at runtime.

Learn more in the [LVGL Open documentation](https://lvgl.io/docs/open/widgets/canvas).

## Properties

### Parts

Style parts of the widget with local style (e.g. `style_bg_color-knob="0xff0000"`) or with style sheets (e.g. `<style name="style_knob" selector="knob">`). See [Styles](https://lvgl.io/syntax/styles) to learn more.

| Part | Description |
| --- | --- |
| `main` | Style the area behind/around the drawn buffer: background, border and padding. The pixels are drawn by the user-set buffer. |

This widget adds no XML properties of its own — it inherits the full API of the [Base Widget (`lv_obj`)](https://lvgl.io/lv_obj). See [API](https://lvgl.io/syntax/api) for how the `<api>` works.

Last updated on
[Calendar (lv_calendar)A month calendar that highlights today and user-defined dates.](https://lvgl.io/docs/pro/built_in_widgets/lv_calendar)[Chart (lv_chart)Visualizes data series as lines, bars or scatter points with axes.](https://lvgl.io/docs/pro/built_in_widgets/lv_chart)
