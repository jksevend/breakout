FILE(COPY shaders/texture2d.frag DESTINATION "${CMAKE_BINARY_DIR}/src/shaders")
FILE(COPY shaders/texture2d.vert DESTINATION "${CMAKE_BINARY_DIR}/src/shaders")

FILE(COPY sprites/background.jpg DESTINATION "${CMAKE_BINARY_DIR}/src/sprites")
FILE(COPY sprites/block.png DESTINATION "${CMAKE_BINARY_DIR}/src/sprites")
FILE(COPY sprites/block_solid.png DESTINATION "${CMAKE_BINARY_DIR}/src/sprites")

FILE(COPY levels/one.lvl DESTINATION "${CMAKE_BINARY_DIR}/src/levels")
FILE(COPY levels/two.lvl DESTINATION "${CMAKE_BINARY_DIR}/src/levels")
FILE(COPY levels/three.lvl DESTINATION "${CMAKE_BINARY_DIR}/src/levels")
FILE(COPY levels/four.lvl DESTINATION "${CMAKE_BINARY_DIR}/src/levels")



