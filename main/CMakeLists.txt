set(src_dirs
            .
            app/src
            ui
)

set(include_dirs
            .
            app/inc
            ui
)

idf_component_register(
            SRC_DIRS ${src_dirs}
            INCLUDE_DIRS ${include_dirs}
)
