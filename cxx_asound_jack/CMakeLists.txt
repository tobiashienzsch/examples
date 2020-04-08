cmake_minimum_required(VERSION 3.13)
project(cxx_linux_asound)

# ALSA
foreach(_target list_devices interrupt alsa_playback)
  add_executable(${_target} "${_target}.cpp")
  target_link_libraries(${_target} PRIVATE -lasound)
  target_compile_features(${_target} PRIVATE cxx_std_17)
endforeach()

# JACK
foreach(_target jack_client jack_capture)
  add_executable(${_target} "${_target}.cpp")
  target_link_libraries(${_target} PRIVATE -ljack -lsndfile -lpthread)
  target_compile_features(${_target} PRIVATE cxx_std_17)
endforeach()