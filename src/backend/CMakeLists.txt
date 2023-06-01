set(CMAKE_MODULE_PATH
        ${CMAKE_CURRENT_SOURCE_DIR}/access
        ${CMAKE_CURRENT_SOURCE_DIR}/catalog
        ${CMAKE_CURRENT_SOURCE_DIR}/executor
        ${CMAKE_CURRENT_SOURCE_DIR}/lib
        ${CMAKE_CURRENT_SOURCE_DIR}/main
        ${CMAKE_CURRENT_SOURCE_DIR}/optimizer
        ${CMAKE_CURRENT_SOURCE_DIR}/port
        ${CMAKE_CURRENT_SOURCE_DIR}/regex
        ${CMAKE_CURRENT_SOURCE_DIR}/storage
        ${CMAKE_CURRENT_SOURCE_DIR}/bootstrap
        ${CMAKE_CURRENT_SOURCE_DIR}/commands
        ${CMAKE_CURRENT_SOURCE_DIR}/libpq
        ${CMAKE_CURRENT_SOURCE_DIR}/nodes
        ${CMAKE_CURRENT_SOURCE_DIR}/parser
        ${CMAKE_CURRENT_SOURCE_DIR}/postmaster
        ${CMAKE_CURRENT_SOURCE_DIR}/rewrite
        ${CMAKE_CURRENT_SOURCE_DIR}/tcop
        ${CMAKE_CURRENT_SOURCE_DIR}/utils
        )
if ("${TIOGA}" STREQUAL "ON")
    list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/tioga)
endif ()

add_subdirectory(access)
add_subdirectory(catalog)
add_subdirectory(executor)
add_subdirectory(lib)
add_subdirectory(main)
add_subdirectory(optimizer)
add_subdirectory(port)
add_subdirectory(regex)
add_subdirectory(storage)
add_subdirectory(bootstrap)
add_subdirectory(commands)
add_subdirectory(libpq)
add_subdirectory(nodes)
add_subdirectory(parser)
add_subdirectory(postmaster)
add_subdirectory(rewrite)
add_subdirectory(tcop)
add_subdirectory(utils)
if ("${TIOGA}" STREQUAL "ON")
    add_subdirectory(tioga)
endif ()

set(postgres_objects
        $<TARGET_OBJECTS:postgres_storage_file>
        $<TARGET_OBJECTS:postgres_access_common>
        $<TARGET_OBJECTS:postgres_access_hash>
        $<TARGET_OBJECTS:postgres_access_heap>
        $<TARGET_OBJECTS:postgres_access_index>
        $<TARGET_OBJECTS:postgres_access_nbtree>
        $<TARGET_OBJECTS:postgres_access_rtree>
        $<TARGET_OBJECTS:postgres_access_transam>
        $<TARGET_OBJECTS:postgres_backend_bootstrap>
        $<TARGET_OBJECTS:postgres_backend_catalog>
        $<TARGET_OBJECTS:postgres_backend_commands>
        $<TARGET_OBJECTS:postgres_backend_executor>
        $<TARGET_OBJECTS:postgres_backend_lib>
        $<TARGET_OBJECTS:postgres_backend_libpq>
        $<TARGET_OBJECTS:postgres_backend_main>
        $<TARGET_OBJECTS:postgres_backend_nodes>
        $<TARGET_OBJECTS:postgres_backend_parser>
        $<TARGET_OBJECTS:postgres_backend_postmaster>
        $<TARGET_OBJECTS:postgres_backend_regex>
        $<TARGET_OBJECTS:postgres_backend_rewrite>
        $<TARGET_OBJECTS:postgres_backend_tcop>
        $<TARGET_OBJECTS:postgres_optimizer_path>
        $<TARGET_OBJECTS:postgres_optimizer_plan>
        $<TARGET_OBJECTS:postgres_optimizer_prep>
        $<TARGET_OBJECTS:postgres_optimizer_util>
        $<TARGET_OBJECTS:postgres_storage_buffer>
        $<TARGET_OBJECTS:postgres_storage_ipc>
        $<TARGET_OBJECTS:postgres_storage_large_object>
        $<TARGET_OBJECTS:postgres_storage_lmgr>
        $<TARGET_OBJECTS:postgres_storage_page>
        $<TARGET_OBJECTS:postgres_storage_smgr>
        $<TARGET_OBJECTS:postgres_utils_adt>
        $<TARGET_OBJECTS:postgres_utils_cache>
        $<TARGET_OBJECTS:postgres_utils_error>
        $<TARGET_OBJECTS:postgres_utils_fmgr>
        $<TARGET_OBJECTS:postgres_utils_hash>
        $<TARGET_OBJECTS:postgres_utils_init>
        $<TARGET_OBJECTS:postgres_utils_mmgr>
        $<TARGET_OBJECTS:postgres_utils_sort>
        $<TARGET_OBJECTS:postgres_utils_time>
        )
set(postgres_LINK_LIBS -lm -ldl)
add_executable(postgres ${postgres_objects})
target_link_libraries(postgres ${postgres_LINK_LIBS})