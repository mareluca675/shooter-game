cmake_minimum_required(VERSION ${CMAKE_VERSION}) # this file comes with cmake

message(VERBOSE "Executing patch step for sheenbidi")

block(SCOPE_FOR VARIABLES)

cmake_language(GET_MESSAGE_LOG_LEVEL _ep_step_log_level)
if(_ep_step_log_level MATCHES "VERBOSE|DEBUG|TRACE")
  set(_ep_step_maybe_command_echo COMMAND_ECHO STDOUT)
else()
  set(_ep_step_maybe_command_echo)
endif()
execute_process(
  WORKING_DIRECTORY "/home/mareluca675/Work/shooter-game/build/_deps/sheenbidi-src"
  COMMAND_ERROR_IS_FATAL LAST
  ${_ep_step_maybe_command_echo}
  COMMAND  [====[/usr/local/bin/cmake]====] [====[-DSHEENBIDI_DIR=/home/mareluca675/Work/shooter-game/build/_deps/sheenbidi-src]====] [====[-P]====] [====[/home/mareluca675/Work/shooter-game/build/_deps/sfml-src/tools/sheenbidi/PatchSheenBidi.cmake]====]
)

endblock()
