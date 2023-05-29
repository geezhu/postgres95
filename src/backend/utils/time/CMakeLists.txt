#This is the main CMAKE for build bin.
AUX_SOURCE_DIRECTORY(${CMAKE_CURRENT_SOURCE_DIR} TGT_time_SRC)

set(TGT_time_INC
        ${CMAKE_SOURCE_DIR}/include
        )

set(time_DEF_OPTIONS ${MACRO_OPTIONS})
set(time_COMPILE_OPTIONS ${OPTIMIZE_OPTIONS} ${OS_OPTIONS} ${PROTECT_OPTIONS} ${WARNING_OPTIONS} ${BIN_SECURE_OPTIONS} ${CHECK_OPTIONS})
set(time_LINK_OPTIONS ${BIN_LINK_OPTIONS})
add_static_objtarget(postgres_utils_time TGT_time_SRC TGT_time_INC "${time_DEF_OPTIONS}" "${time_COMPILE_OPTIONS}" "${time_LINK_OPTIONS}")

