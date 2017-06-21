cmake_minimum_required( VERSION 3.0 FATAL_ERROR )
set( CMAKE_VERBOSE_MAKEFILE ON )

project( SilverSprintApp )

get_filename_component( CINDER_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../../../.." ABSOLUTE )
get_filename_component( APP_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../../" ABSOLUTE )

include( "${CINDER_PATH}/proj/cmake/modules/cinderMakeApp.cmake" )

ci_make_app(
	SOURCES     ${APP_PATH}/src/SilverSprintApp.cpp
	SOURCES     ${APP_PATH}/src/app/GFXMain.cpp
	SOURCES     ${APP_PATH}/src/app/SSManager.cpp
	SOURCES     ${APP_PATH}/src/data/GFXGlobal.cpp
	SOURCES     ${APP_PATH}/src/data/Model.cpp
	SOURCES     ${APP_PATH}/src/data/SerialReader.cpp
	SOURCES     ${APP_PATH}/src/data/StateManager.cpp
	SOURCES     ${APP_PATH}/src/ui/BaseButton.cpp
	SOURCES     ${APP_PATH}/src/ui/CiTextField.cpp
	SOURCES     ${APP_PATH}/src/ui/CountDownGfx.cpp
	SOURCES     ${APP_PATH}/src/ui/NumStepper.cpp
	SOURCES     ${APP_PATH}/src/ui/PlayerGfx.cpp
	SOURCES     ${APP_PATH}/src/ui/RaceText.cpp
	SOURCES     ${APP_PATH}/src/ui/StartStopButton.cpp
	SOURCES     ${APP_PATH}/src/ui/ThickLine.cpp
	SOURCES     ${APP_PATH}/src/ui/ToggleBtn.cpp
	SOURCES     ${APP_PATH}/src/ui/WinnerModal.cpp
	SOURCES     ${APP_PATH}/src/views/NavBarView.cpp
	SOURCES     ${APP_PATH}/src/views/RaceView.cpp
	SOURCES     ${APP_PATH}/src/views/RosterView.cpp
	SOURCES     ${APP_PATH}/src/views/SettingsView.cpp
	SOURCES     ${APP_PATH}/blocks/ParticleSystem/ParticleSystem.cpp
	SOURCES     ${APP_PATH}/blocks/Serial/lib/serial/src/serial.cc
	SOURCES     ${APP_PATH}/blocks/Serial/lib/serial/src/impl/list_ports/list_ports_linux.cc
	SOURCES     ${APP_PATH}/blocks/Serial/lib/serial/src/impl/unix.cc
	SOURCES     ${APP_PATH}/blocks/Serial/src/SerialDevice.cpp
	SOURCES     ${APP_PATH}/blocks/OSC/src/cinder/osc/Osc.cpp
	CINDER_PATH ${CINDER_PATH}
)
