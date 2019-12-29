project(SBE-external-iniparser)

set(SBE_INIPARSER_PATH ${CMAKE_SOURCE_DIR}/../iniparser
	CACHE PATH
	"Path to the iniparser library sources"
)

add_library(${PROJECT_NAME} STATIC)

aux_source_directory(${SBE_INIPARSER_PATH}/src PROJECT_SOURCES)

target_sources(${PROJECT_NAME}
	PRIVATE ${PROJECT_SOURCES}
)

include_directories(${SBE_INIPARSER_PATH}/src)