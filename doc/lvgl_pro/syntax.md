---
url: https://lvgl.io/docs/pro/syntax
title: UI Elements
---

# UI Elements

Source: <https://lvgl.io/docs/pro/syntax>

# UI Elements
Complete reference for all UI elements, components, widgets, and features available in LVGL Pro Editor.
Copy MarkdownOpen
LVGL Pro Editor uses XML to define user interfaces, providing a declarative format for describing UI components, screens, layouts, and animations with full code generation support.
This section covers everything from basic building blocks to advanced styling and animation techniques.

## Understanding XML

[### Introduction LVGL's XML
Learn the fundamentals of XML format and how it powers LVGL Pro Editor's UI definition system.](https://lvgl.io/docs/pro/syntax/introduction)[### XML Syntax Overview
Explore XML syntax rules, element structure, attributes, and best practices for writing clean UI definitions.](https://lvgl.io/docs/pro/syntax/overview)[### Project File
Configure `project.xml` with the LVGL version, exported project name, and display targets used to preview screens.](https://lvgl.io/docs/pro/syntax/project)

## UI Elements

[###
ComponentsCreate reusable UI modules with custom properties, styles, and API definitions. The foundation for building complex interfaces.](https://lvgl.io/docs/pro/syntax/components)[###
WidgetsReference guide for all available widgets and UI controls. Learn how to create custom widgets with XML parsers.](https://lvgl.io/docs/pro/syntax/widgets)[###
ScreensOrganize your UI content with screens. Manage navigation, permanent screens, and screen lifecycle.](https://lvgl.io/docs/pro/syntax/screens)

## Visuals & Styling

[###
ViewDefine the visual structure and layout of components, widgets, and screens using the view tag.](https://lvgl.io/docs/pro/syntax/view)[###
StylesMaster style sheets, local styles, gradients, and conditional style binding with subjects for dynamic theming.](https://lvgl.io/docs/pro/syntax/styles)[###
ConstantsDefine reusable constants for colors, spacing, strings, and other values to maintain consistency.](https://lvgl.io/docs/pro/syntax/constants)[###
AnimationsCreate timeline-based animations with multiple steps. Include timelines from other components for complex effects.](https://lvgl.io/docs/pro/syntax/animations)

## Assets

[###
ImagesRegister and reference images as named external resources in your XML files. Map images from files or compiled data arrays.](https://lvgl.io/docs/pro/syntax/images)[###
FontsRegister and configure fonts as external resources. Select font engines and name them for reference in XML files.](https://lvgl.io/docs/pro/syntax/fonts)

## Interaction & Configuration

[###
APIDefine custom APIs for widgets and components with properties, parameters, enums, and elements.](https://lvgl.io/docs/pro/syntax/api)[###
Events in XMLHandle user interactions with event callbacks, screen navigation, and subject value updates on triggers.](https://lvgl.io/docs/pro/syntax/events)[###
Data BindingUse subjects to bind data to widget properties, creating dynamic UIs that automatically update when values change.](https://lvgl.io/docs/pro/syntax/data-binding)[###
Language TranslationsImplement multi-language support using the XML translation module to define and manage translated strings efficiently.](https://lvgl.io/docs/pro/syntax/translations)

## Test & Preview

[###
UI TestingWrite comprehensive UI tests using XML to verify functionality and visual consistency of your LVGL interfaces.](https://lvgl.io/docs/pro/syntax/testing)[###
PreviewConfigure preview settings for components and widgets in the UI Editor with multiple preview contexts.](https://lvgl.io/docs/pro/syntax/preview)

Last updated on
[Learn by ExamplesExplore interactive tutorials and hands-on examples to master LVGL Pro Editor features from Hello World to advanced animations.](https://lvgl.io/docs/pro/learn-by-examples)[IntroductionLearn how LVGL's XML format enables declarative UI development with version control, reusability, and runtime flexibility for embedded applications.](https://lvgl.io/docs/pro/syntax/introduction)
