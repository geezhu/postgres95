#This is the main CMAKE for build bin.
AUX_SOURCE_DIRECTORY(${CMAKE_CURRENT_SOURCE_DIR} TGT_commands_SRC)

set(TGT_commands_INC
        ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_SOURCE_DIR}/include/port/${PORTNAME}
        )

set(commands_DEF_OPTIONS ${MACRO_OPTIONS})
set(commands_COMPILE_OPTIONS ${OPTIMIZE_OPTIONS} ${OS_OPTIONS} ${PROTECT_OPTIONS} ${WARNING_OPTIONS} ${BIN_SECURE_OPTIONS} ${CHECK_OPTIONS})
set(commands_LINK_OPTIONS ${BIN_LINK_OPTIONS})
add_static_objtarget(postgres_commands TGT_commands_SRC TGT_commands_INC "${commands_DEF_OPTIONS}" "${commands_COMPILE_OPTIONS}" "${commands_LINK_OPTIONS}")

