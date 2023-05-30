# src/backend/catalog/CMakeLists.txt
option(ALLOW_PG_GROUP "ALLOW_PG_GROUP" OFF)
set(BKIOPTS "")
if ("${ALLOW_PG_GROUP}" STREQUAL "ON")
    set(BKIOPTS -DALLOW_PG_GROUP)
endif ()