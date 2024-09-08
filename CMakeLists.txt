cmake_minimum_required(VERSION 3.18)

project(DashboardApp LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

set(HTTP_SERVER_PORT 8080)
set(CLIENTID "")
set(CLIENT_SECRET "")
set(CLIENTID_SECRET "${CLIENTID}:${CLIENT_SECRET}")
set(ELM_DEVICE_PRODUCT_ID 28932)
set(ENABLE_CAR_CRUISING_MOCKS true)

set(SPOTIFY_API_FLAGS "")
list(APPEND SPOTIFY_API_FLAGS  "-DHTTP_SERVER_PORT=${HTTP_SERVER_PORT}")
list(APPEND SPOTIFY_API_FLAGS "-DCLIENTID=${CLIENTID}")
list(APPEND SPOTIFY_API_FLAGS "-DCLIENTID_SECRET=${CLIENTID_SECRET}")

# decide between mocks and elm files
if(ENABLE_CAR_CRUISING_MOCKS)
    set(ELM_DEVICE_PRODUCT_ID_FLAG "")
    set(CAR_CRUISING_MOCK_FLAG "-DCAR_CRUISING_MOCK")
    set(CAR_CRUISING_FILES
        src/speedmock.h             src/speedmock.cpp
        src/rpmmock.h               src/rpmmock.cpp
    )
else()
    set(ELM_DEVICE_PRODUCT_ID_FLAG "-DELM_DEVICE_PRODUCT_ID=${ELM_DEVICE_PRODUCT_ID}")
    set(CAR_CRUISING_MOCK_FLAG "")
    set(CAR_CRUISING_FILES
        src/elm/serialdevice.h      src/elm/serialdevice.cpp
        src/elm/elm.h               src/elm/elm.cpp
        src/elmconnection.h         src/elmconnection.cpp
        src/rpmexposer.h            src/rpmexposer.cpp
        src/speedexposer.h          src/speedexposer.cpp
    )
endif()

string(REPLACE ";" " " FLAGS "${SPOTIFY_API_FLAGS} ${ELM_DEVICE_PRODUCT_ID_FLAG} ${CAR_CRUISING_MOCK_FLAG}")

# Check the processor architecture
if(CMAKE_SYSTEM_PROCESSOR MATCHES "arm")
    set(CMAKE_CXX_FLAGS "-lpigpio -DPI_ARM ${FLAGS}")
    set(BUTTON_FILES
        src/pigpiobutton.h
        src/pigpiobutton.cpp
        src/mockbutton.h
        src/mockbutton.cpp
    )
elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "x86_64")
    set(CMAKE_CXX_FLAGS "${FLAGS}")
    set(BUTTON_FILES
        src/mockbutton.h
        src/mockbutton.cpp
    )
else()
    message(FATAL_ERROR "This architecture wasn't tested: ${CMAKE_SYSTEM_PROCESSOR}, take a look at the makefile if you still want to try to compile")
endif()

find_package(Qt6 REQUIRED COMPONENTS Gui Qml Quick Network Core5Compat HttpServer SerialPort DBus)
qt_add_executable(DashboardApp
    src/main.cpp
    src/clockexposer.h                      src/clockexposer.cpp
    src/bluetoothmanager.h                  src/bluetoothmanager.cpp
    src/utils.h                             src/utils.cpp
    src/spotifyauthmanager.h                src/spotifyauthmanager.cpp
    src/qrcodegen.h                         src/qrcodegen.cpp
    src/qrimage.h                           src/qrimage.cpp
    src/qrimageprovider.h                   src/qrimageprovider.cpp
    src/spotifywebapi.h                     src/spotifywebapi.cpp
    src/spotifylyricsapi.h                  src/spotifylyricsapi.cpp
    src/carcruisingdataexposer.h            src/carcruisingdataexposer.cpp
    src/bluezdeviceconnection.h             src/bluezdeviceconnection.cpp
    src/bluezdevicepairer.h                 src/bluezdevicepairer.cpp
    src/bluezdevicemonitor.h                src/bluezdevicemonitor.cpp
    src/ibutton.h
    src/bluetoothagent.h
    ${CAR_CRUISING_FILES}
    ${BUTTON_FILES}
)

string(TOLOWER ${CMAKE_BUILD_TYPE} build_type)
if (build_type STREQUAL release)
    target_compile_definitions(${PROJECT_NAME} PUBLIC
        QT_NO_DEBUG_OUTPUT
        QT_NO_WARNING_OUTPUT
        )
endif()


qt_add_resources(DashboardApp "configuration"
    PREFIX "/"
    FILES
        qtquickcontrols2.conf
)

add_subdirectory(http)

target_link_libraries(DashboardApp PRIVATE
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Gui
    Qt${QT_VERSION_MAJOR}::Quick
    Qt${QT_VERSION_MAJOR}::Qml
    Qt${QT_VERSION_MAJOR}::Network
    Qt${QT_VERSION_MAJOR}::Core5Compat
    Qt${QT_VERSION_MAJOR}::HttpServer
    Qt${QT_VERSION_MAJOR}::SerialPort
    Qt${QT_VERSION_MAJOR}::DBus
    http
)

include(${CMAKE_CURRENT_SOURCE_DIR}/qmlmodules)

install(TARGETS DashboardApp
    RUNTIME DESTINATION "${INSTALL_EXAMPLEDIR}"
    BUNDLE DESTINATION "${INSTALL_EXAMPLEDIR}"
    LIBRARY DESTINATION "${INSTALL_EXAMPLEDIR}"
)

# Make Qt Creator aware of where the QML modules live
set (_QML_IMPORT_PATHS "")

## Add new module paths here.
list (APPEND _QML_IMPORT_PATHS ${CMAKE_CURRENT_SOURCE_DIR}/asset_imports)
list (APPEND _QML_IMPORT_PATHS ${CMAKE_CURRENT_SOURCE_DIR}/imports)

set (
    QML_IMPORT_PATH
    ${_QML_IMPORT_PATHS}
    CACHE
    STRING
    "Path used to locate CMake modules by Qt Creator"
    FORCE
)
