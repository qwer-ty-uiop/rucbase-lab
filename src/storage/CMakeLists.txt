set(SOURCES 
        disk_manager.cpp 
        buffer_pool_manager.cpp
)
add_library(storage STATIC ${SOURCES})
target_link_libraries(storage clock_replacer lru_replacer)
