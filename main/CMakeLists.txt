set(src_dirs
            .
            app/src
)

set(include_dirs
            .
            app/inc
)

idf_component_register(
            SRC_DIRS ${src_dirs}
            INCLUDE_DIRS ${include_dirs}
)
