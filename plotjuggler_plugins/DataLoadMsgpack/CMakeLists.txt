include_directories(  ../ )

add_definitions(${QT_DEFINITIONS})
add_definitions(-DQT_PLUGIN)

SET(SRC dataload_msgpack.cpp msgpack_parser.cpp)

add_library(DataLoadMsgpack SHARED ${SRC})
target_link_libraries(DataLoadMsgpack ${Qt5Widgets_LIBRARIES} ${Qt5Xml_LIBRARIES} plotjuggler_base QCodeEditor)

install(TARGETS DataLoadMsgpack DESTINATION ${PJ_PLUGIN_INSTALL_DIRECTORY})