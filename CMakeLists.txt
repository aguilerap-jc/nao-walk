cmake_minimum_required(VERSION 2.8)
project(fSheldonWalk)

find_package(qibuild)

option(BUMPER_IS_REMOTE "module is compiles as a remote module (ON or OFF)"ON)

set(_srcs
	main.cpp
	bumper.h
	bumper.cpp
	CaminadoNAO.cpp
	CaminadoNAO.h
)

if(BUMPER_IS_REMOTE)
	add_definitions(" -DBUMPER_IS_REMOTE")
	qi_create_bin(fSheldonWalk ${_srcs})
else()
	qi_create_lib(fSheldonWalk SHARED %{_srcs} SUBFOLDER naoqi)
endif()

qi_use_lib(fSheldonWalk OPENCV2_CORE OPENCV2_HIGHGUI OPENCV2_IMGPROC)
qi_use_lib(fSheldonWalk ALCOMMON ALPROXIES)

