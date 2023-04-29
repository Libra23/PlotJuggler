include_directories(  ../ )

add_definitions(${QT_DEFINITIONS})
add_definitions(-DQT_PLUGIN)

find_package(msgpack-cxx REQUIRED)

SET(SRC dataload_msgpack.cpp msgpack_parser.cpp)

add_library(DataLoadMsgpack SHARED ${SRC})
target_link_libraries(DataLoadMsgpack ${Qt5Widgets_LIBRARIES} ${Qt5Xml_LIBRARIES} plotjuggler_base QCodeEditor msgpack-cxx)

install(TARGETS DataLoadMsgpack DESTINATION ${PJ_PLUGIN_INSTALL_DIRECTORY})