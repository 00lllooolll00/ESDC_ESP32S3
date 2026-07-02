---
url: https://lvgl.io/docs/pro/built_in_widgets/lv_button
title: Button (lv_button)
---

# Button (lv_button)

Source: <https://lvgl.io/docs/pro/built_in_widgets/lv_button>

[Built-in Widgets](https://lvgl.io/docs/pro/built_in_widgets)

# Button (lv_button)
A clickable Base Widget with defaults tuned for its role.
Copy MarkdownOpen

## Overview

A Button is a clickable [Base Widget](https://lvgl.io/lv_obj) with defaults tuned for its role: it is not scrollable, is added to the default input group, and sizes itself to its content. Place a [Label](https://lvgl.io/lv_label) or image inside it for the caption.

Learn more in the [LVGL Open documentation](https://lvgl.io/docs/open/widgets/button).

PreviewCXML
[lv_example_button.c](https://github.com/lvgl/lvgl_editor/blob/master/docs/examples/screens/lv_example_button.c)

## Properties

### Parts

Style parts of the widget with local style (e.g. `style_bg_color-knob="0xff0000"`) or with style sheets (e.g. `<style name="style_knob" selector="knob">`). See [Styles](https://lvgl.io/syntax/styles) to learn more.

| Part | Description |
| --- | --- |
| `main` | Style the whole button: background, border, outline, shadow and padding. Combine with state selectors (pressed, checked, disabled, focused) for feedback. |

This widget adds no XML properties of its own — it inherits the full API of the [Base Widget (`lv_obj`)](https://lvgl.io/lv_obj). See [API](https://lvgl.io/syntax/api) for how the `<api>` works.

Last updated on
[Bar (lv_bar)A background with an indicator whose length represents a value within a range.](https://lvgl.io/docs/pro/built_in_widgets/lv_bar)[Button Matrix (lv_buttonmatrix)Many buttons in rows and columns drawn virtually from a single text map.](https://lvgl.io/docs/pro/built_in_widgets/lv_buttonmatrix)
