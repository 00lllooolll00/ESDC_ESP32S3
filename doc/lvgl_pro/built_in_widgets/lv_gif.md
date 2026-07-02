---
url: https://lvgl.io/docs/pro/built_in_widgets/lv_gif
title: GIF (lv_gif)
---

# GIF (lv_gif)

Source: <https://lvgl.io/docs/pro/built_in_widgets/lv_gif>

[Built-in Widgets](https://lvgl.io/docs/pro/built_in_widgets)

# GIF (lv_gif)
Plays animated GIF images using LVGL's built-in decoder.
Copy MarkdownOpen

## Overview

The GIF widget plays animated GIF images using LVGL's built-in AnimatedGIF decoder. The source can be an embedded array or a file.

Learn more in the [LVGL Open documentation](https://lvgl.io/docs/open/widgets/gif).

## Properties

### Parts

Style parts of the widget with local style (e.g. `style_bg_color-knob="0xff0000"`) or with style sheets (e.g. `<style name="style_knob" selector="knob">`). See [Styles](https://lvgl.io/syntax/styles) to learn more.

| Part | Description |
| --- | --- |
| `main` | Style the GIF: `image_recolor`, `image_recolor_opa` and `image_opa`. Background, border, etc can be added too. |

Properties below are the widget's XML [`<api>`](https://lvgl.io/syntax/api); see [API](https://lvgl.io/syntax/api) for how properties, parameters and elements work.

| Property | Type | Description |
| --- | --- | --- |
| `src` | `image` | The source for the gif (raw data or a file) |
| `loop_count` | `int` | Number of repeats. If not set the information form the gif will be used |

Last updated on
[Drop-down List (lv_dropdown)Shows a selected value and opens a list to pick one option.](https://lvgl.io/docs/pro/built_in_widgets/lv_dropdown)[Image (lv_image)Displays images from arrays or files, with recolor, scale and rotation.](https://lvgl.io/docs/pro/built_in_widgets/lv_image)
