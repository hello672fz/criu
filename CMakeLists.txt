cmake_minimum_required(VERSION 3.16)
project(cxl-exp)
set(CMAKE_CXX_STANDARD 17)

set(EXP00_BUILD ON)

if (EXP00_BUILD)
    message("exp00 building...")
    add_executable(00_get_numa_info cxl-exp/00.numa/get_numa_info.c)
    target_link_libraries(00_get_numa_info numa)

    add_executable(00_alloc_memory_with_numa cxl-exp/00.numa/alloc_memory_with_numa.c)
    target_link_libraries(00_alloc_memory_with_numa numa)

    add_executable(00_alloc_memory_wo_numa cxl-exp/00.numa/alloc_memory_wo_numa.c)

    add_executable(00_get_mem_info cxl-exp/00.numa/get_mem_info.c)
    target_link_libraries(00_get_mem_info numa)
endif ()
