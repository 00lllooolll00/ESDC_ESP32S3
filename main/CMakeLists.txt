set(src_dirs
            .
            app/src
            ui
            ui/screens
            ui/components
)

set(include_dirs
            .
            app/inc
            ui
            ui/screens
            ui/components
)

idf_component_register(
            SRC_DIRS ${src_dirs}
            INCLUDE_DIRS ${include_dirs}
)
