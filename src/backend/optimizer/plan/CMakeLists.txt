#This is the main CMAKE for build bin.
AUX_SOURCE_DIRECTORY(${CMAKE_CURRENT_SOURCE_DIR} TGT_plan_SRC)

set(TGT_plan_INC
        ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_SOURCE_DIR}/include/port/${PORTNAME}
        )

set(plan_DEF_OPTIONS ${MACRO_OPTIONS})
set(plan_COMPILE_OPTIONS ${OPTIMIZE_OPTIONS} ${OS_OPTIONS} ${PROTECT_OPTIONS} ${WARNING_OPTIONS} ${BIN_SECURE_OPTIONS} ${CHECK_OPTIONS})
set(plan_LINK_OPTIONS ${BIN_LINK_OPTIONS})
add_static_objtarget(postgres_optimizer_plan TGT_plan_SRC TGT_plan_INC "${plan_DEF_OPTIONS}" "${plan_COMPILE_OPTIONS}" "${plan_LINK_OPTIONS}")

