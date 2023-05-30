execute_process(
    COMMAND sh Gen_fmgrtab.sh ${CMAKE_SOURCE_DIR}/include/catalog/pg_proc.h
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)
execute_process(
    COMMAND mv fmgr.h ${CMAKE_SOURCE_DIR}/include/fmgr.h
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)
set(CMAKE_MODULE_PATH
        ${CMAKE_CURRENT_SOURCE_DIR}/adt
        ${CMAKE_CURRENT_SOURCE_DIR}/cache
        ${CMAKE_CURRENT_SOURCE_DIR}/error
        ${CMAKE_CURRENT_SOURCE_DIR}/fmgr
        ${CMAKE_CURRENT_SOURCE_DIR}/hash
        ${CMAKE_CURRENT_SOURCE_DIR}/init
        ${CMAKE_CURRENT_SOURCE_DIR}/mmgr
        ${CMAKE_CURRENT_SOURCE_DIR}/sort
        ${CMAKE_CURRENT_SOURCE_DIR}/time
        )
add_subdirectory(adt)
add_subdirectory(cache)
add_subdirectory(error)
add_subdirectory(fmgr)
add_subdirectory(hash)
add_subdirectory(init)
add_subdirectory(mmgr)
add_subdirectory(sort)
add_subdirectory(time)