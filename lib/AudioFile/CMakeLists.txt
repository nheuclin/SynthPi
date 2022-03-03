cmake_minimum_required (VERSION 3.12)

add_library (audiofile AudioFile.hpp)

set_target_properties(audiofile PROPERTIES PUBLIC_HEADER AudioFile.hpp)
set_target_properties(audiofile PROPERTIES LINKER_LANGUAGE CXX)