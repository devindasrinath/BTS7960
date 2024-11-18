add_library( bts7960
    bts7960.h
    bts7960.c
    )

target_link_libraries(bts7960 pico_stdlib hardware_pwm hardware_adc)
target_include_directories(bts7960 PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")