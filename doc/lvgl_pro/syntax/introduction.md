---
url: https://lvgl.io/docs/pro/syntax/introduction
title: Introduction
---

# Introduction

Source: <https://lvgl.io/docs/pro/syntax/introduction>

[XML Syntax](https://lvgl.io/docs/pro/syntax)

# Introduction
Learn how LVGL's XML format enables declarative UI development with version control, reusability, and runtime flexibility for embedded applications.
Copy MarkdownOpen
Describe your embedded UIs in XML instead of code, combining the best of both worlds: the clarity of markup with the power of modern development tools. LVGL's XML Module brings declarative UI programming to embedded systems.

## Why Use XML?

XML offers powerful advantages over traditional drag-and-drop or pure code-based approaches:

- HTML-like syntax - Familiar to web developers, easy to learn and read

- Version control friendly - Plain text with readable diffs, no binary files

- Share and collaborate - Copy, paste, and send XML snippets effortlessly

- Reusable components - Build libraries of UI patterns for consistency

- Automation ready - Scripts and CI/CD pipelines can process XML automatically

- AI-powered workflows - AI tools can read, generate, and refactor your UIs

- Runtime flexibility - Parse and load XML at runtime without recompiling

- Cross-platform - The same XML works across all LVGL targets

- Fast iteration - Quicker to type than drag-and-drop editing

## Working with XML

You can write XML by hand, but we recommend using LVGL Pro's Editor for a better experience.

**Recommended: Use LVGL Pro's Editor**
The Editor provides live preview, inspector tools, code generation, auto-completion, syntax highlighting, validation, online preview and sharing for team feedback, and Figma integration to convert designs directly to XML.
[Learn more about the Editor](https://lvgl.io/editor/overview)

## Core Concepts

XML UIs are built from three fundamental elements: **Widgets**, **Components**, and **Screens**. Understanding the differences is key to effective UI design.

### Widgets
Fundamental building blocks that compile into C code.
`<widget>`
Characteristics

- Similar to LVGL's built-in widgets
- Support custom and complex logic
- Have a large, customizable API with `set/get/add` functions
- Compound widgets can contain other widgets (e.g., Tabview, Dropdown)
- Cannot be loaded from XML at runtime (custom code isn't loadable)
- Use a custom XML parser to map properties to API functions

When to use
Building reusable custom UI elements that need advanced logic or compiled performance.

### Components
Lightweight, XML-only UI elements that load at runtime.
`<component>`
Characteristics

- Built from Widgets and/or other Components
- Pure XML—no custom C code (but can call C functions as callbacks)
- Support data bindings, animations, and styling
- Load from XML at runtime
- Can have a simple API to pass properties to children (e.g., `btn_text`)
- Can contain Widgets and/or other Components

LVGL reads Component XML files, learns their structure, and creates instances as needed within Screens and other Components.

When to use
Building reusable UI patterns, styled configurations, and dynamic UI elements.

### Screens
Top-level containers for complete UI views.
`<screen>`
Characteristics

- Built from Widgets and/or Components
- Define complete visual layouts
- Load at runtime (visual only, no custom code)
- No complex API—properties are for visual styling
- Can be referenced in screen load events and navigation

When to use
Defining application screens, pages, and full-screen views.

## Shared Resources with globals.xml

The `globals.xml` file defines project-wide resources accessible throughout your UI:

- Styles - Reusable styling definitions

- Constants - Project-wide values and settings

- Images - Image assets and sprites

- Fonts - Font definitions and configurations

- Subjects - Data binding sources for reactive updates

You can load multiple `globals.xml` files if needed. They merge into the same global scope, preventing duplicate items.

## Using XML Files

XML files serve two primary purposes:

- Runtime loading - LVGL parses and loads XML directly at runtime

- Code generation - Convert XML to optimized C code using the Editor or CLI

Both workflows are detailed in the Integration section.

Last updated on
[UI ElementsComplete reference for all UI elements, components, widgets, and features available in LVGL Pro Editor.](https://lvgl.io/docs/pro/syntax)[OverviewMaster XML syntax rules, naming conventions, and property types used in LVGL Pro Editor for defining UI components.](https://lvgl.io/docs/pro/syntax/overview)
