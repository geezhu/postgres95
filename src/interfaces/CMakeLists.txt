set(CMAKE_MODULE_PATH
        ${CMAKE_CURRENT_SOURCE_DIR}/libpgtcl
        ${CMAKE_CURRENT_SOURCE_DIR}/libpq
        )

add_subdirectory(libpgtcl)
add_subdirectory(libpq)
