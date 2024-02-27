file(GLOB_RECURSE SOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
file(GLOB_RECURSE HEADER_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.h)

add_executable(${PROJECT_NAME} ${HEADER_FILES} ${SOURCE_FILES} ../main.cpp)

target_link_libraries(${PROJECT_NAME} PUBLIC ${OPENGL_LIBRARIES} glad glm::glm glfw spdlog::spdlog freetype)
target_include_directories(${PROJECT_NAME} PRIVATE ${GLM_INCLUDE_DIRS})

include_directories(${PROJECT_NAME} ../deps/stb_image)
