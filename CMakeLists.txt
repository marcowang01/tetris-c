cmake_minimum_required(VERSION 3.3)
project(final-project CXX)
include(.cs211/cmake/CMakeLists.txt)

# Defines a variable to stand for the .cxx files that implement
# the model.
set(MODEL_SRC
        src/player.cxx
        src/position_set.cxx
        src/board.cxx
        src/model.cxx
        src/game_config.cxx
        src/ball.cxx)


# files and the common model files.
add_program(main
        src/main.cxx
        src/controller.cxx
        src/view.cxx
        ${MODEL_SRC})
target_link_libraries(main ge211)

# Adds a test program named `model_test` built from the listed
# source file and the common model files.
add_test_program(model_test
        test/model_test.cxx
        ${MODEL_SRC})
target_link_libraries(model_test ge211)

# vim: ft=cmake
