set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

set(SOURCES
	"${SOURCE_DIR}/main.cpp"

	"${SOURCE_DIR}/Managers/VideoManager.cpp"
	"${SOURCE_DIR}/Managers/UIManager.cpp"

	"${SOURCE_DIR}/Rendering/SimpleMeshMaterial.cpp"
	"${SOURCE_DIR}/Rendering/SkyboxMaterial.cpp"
	"${SOURCE_DIR}/Rendering/TextureStreamer.cpp"

	"${SOURCE_DIR}/Scene/DebugCamera.cpp"
	"${SOURCE_DIR}/Scene/HeadSet.cpp"
	"${SOURCE_DIR}/Scene/MainScene.cpp"
	"${SOURCE_DIR}/Scene/SteamVRObject.cpp"

	"${SOURCE_DIR}/UI/Widget.cpp"

	"${SOURCE_DIR}/Video/AudioStream.cpp"
	"${SOURCE_DIR}/Video/Video.cpp"
)

add_definitions(
	-D_SILENCE_ALL_CXX17_DEPRECATION_WARNINGS
	-D_CRT_SECURE_NO_WARNINGS
	-DSFML_STATIC
	-DGLEW_STATIC
)

link_directories(${THIRDPARTY_LIB_DIR})

add_executable(vrtour ${SOURCES})
target_include_directories(vrtour PRIVATE 
	${INCLUDE_DIR} 
	${SOURCE_DIR} 
	${ENGINE_INCLUDE_DIR} 
	${THIRDPARTY_INCLUDE_DIR}
)

target_compile_features(vrtour PUBLIC cxx_std_17)
set_target_properties(vrtour PROPERTIES CXX_EXTENSIONS OFF)

target_link_libraries(vrtour PUBLIC engine
	opengl32
	sfml-graphics-s
	sfml-network-s
	sfml-window-s
	sfml-system-s
	sfml-audio-s
	ws2_32
	freetype
	winmm
	gdi32
	ogg
	openal32
	flac
	vorbisenc
	vorbisfile
	vorbis
	glew32s
	openvr_api
	avcodec
	avdevice
	avfilter
	avformat
	avutil
	postproc
	swresample
	swscale
)