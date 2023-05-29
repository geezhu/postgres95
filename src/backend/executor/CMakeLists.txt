#This is the main CMAKE for build bin.
AUX_SOURCE_DIRECTORY(${CMAKE_CURRENT_SOURCE_DIR} TGT_executor_SRC)

set(TGT_executor_INC
        ${CMAKE_SOURCE_DIR}/include
        )

set(executor_DEF_OPTIONS ${MACRO_OPTIONS})
set(executor_COMPILE_OPTIONS ${OPTIMIZE_OPTIONS} ${OS_OPTIONS} ${PROTECT_OPTIONS} ${WARNING_OPTIONS} ${BIN_SECURE_OPTIONS} ${CHECK_OPTIONS})
set(executor_LINK_OPTIONS ${BIN_LINK_OPTIONS})
add_static_objtarget(postgres_backend_executor TGT_executor_SRC TGT_executor_INC "${executor_DEF_OPTIONS}" "${executor_COMPILE_OPTIONS}" "${executor_LINK_OPTIONS}")

