project(cxx_linalg_tests)

function(linalg_add_test _target)
  add_executable(${_target}_test "${_target}_test.cpp")
  add_test(NAME ${_target}_test COMMAND ${_target}_test)
  target_link_libraries(${_target}_test PRIVATE etl)
endfunction()

linalg_add_test(scale)
linalg_add_test(swap_elements)
