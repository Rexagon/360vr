set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

set(SOURCES
	"${SOURCE_DIR}/main.cpp"

	"${SOURCE_DIR}/Managers/VideoManager.cpp"
	"${SOURCE_DIR}/Managers/UIManager.cpp"

	"${SOURCE_DIR}/Rendering/SimpleMeshMaterial.cpp"
	"${SOURCE_DIR}/Rendering/SkyboxMaterial.cpp"
	"${SOURCE_DIR}/Rendering/TextMaterial.cpp"
	"${SOURCE_DIR}/Rendering/TextureStreamer.cpp"
	"${SOURCE_DIR}/Rendering/WidgetMaterial.cpp"
	"${SOURCE_DIR}/Rendering/WireFrameMaterial.cpp"

	"${SOURCE_DIR}/Scene/CollisionWorld.cpp"
	"${SOURCE_DIR}/Scene/DebugCamera.cpp"
	"${SOURCE_DIR}/Scene/HeadSet.cpp"
	"${SOURCE_DIR}/Scene/MainScene.cpp"
	"${SOURCE_DIR}/Scene/SteamVRObject.cpp"

	"${SOURCE_DIR}/UI/RectangleWidget.cpp"
	"${SOURCE_DIR}/UI/TextWidget.cpp"

	"${SOURCE_DIR}/Video/AudioPlayer.cpp"
	"${SOURCE_DIR}/Video/AudioStream.cpp"
	"${SOURCE_DIR}/Video/Video.cpp"
	"${SOURCE_DIR}/Video/VideoState.cpp"
	"${SOURCE_DIR}/Video/VideoStream.cpp"
)

add_definitions(
	-D_SILENCE_ALL_CXX17_DEPRECATION_WARNINGS
	-D_CRT_SECURE_NO_WARNINGS
	-DASIO_STANDALONE
	-DASIO_HAS_STD_ADDRESSOF 
	-DASIO_HAS_STD_ARRAY
	-DASIO_HAS_CSTDINT
	-DASIO_HAS_STD_SHARED_PTR
	-DASIO_HAS_STD_TYPE_TRAITS
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

	BulletDynamics
	BulletCollision
	LinearMath
)
