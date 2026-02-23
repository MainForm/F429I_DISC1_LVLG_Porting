add_library(sdramIS42S16400J 
    STATIC 
        ${CMAKE_CURRENT_SOURCE_DIR}/Src/IS42S16400J_7TL.c
)

target_include_directories(sdramIS42S16400J
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/Inc
)

target_link_libraries(sdramIS42S16400J 
    PUBLIC
        stm32cubemx
)