set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(ENGINE_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

set(SOURCES
	"${SOURCE_DIR}/Core/Core.cpp"

	"${SOURCE_DIR}/Managers/FileManager.cpp"
	"${SOURCE_DIR}/Managers/FontManager.cpp"
	"${SOURCE_DIR}/Managers/InputManager.cpp"
	"${SOURCE_DIR}/Managers/MeshManager.cpp"
	"${SOURCE_DIR}/Managers/RenderingManager.cpp"
	"${SOURCE_DIR}/Managers/SceneManager.cpp"
	"${SOURCE_DIR}/Managers/ShaderManager.cpp"
	"${SOURCE_DIR}/Managers/TextureManager.cpp"
	"${SOURCE_DIR}/Managers/VRManager.cpp"
	"${SOURCE_DIR}/Managers/WindowManager.cpp"

	"${SOURCE_DIR}/Rendering/ForwardRenderer.cpp"
	"${SOURCE_DIR}/Rendering/FrameBuffer.cpp"
	"${SOURCE_DIR}/Rendering/Renderer.cpp"
	"${SOURCE_DIR}/Rendering/RenderingState.cpp"
	"${SOURCE_DIR}/Rendering/UIRenderer.cpp"

	"${SOURCE_DIR}/Resources/InputAxis.cpp"
	"${SOURCE_DIR}/Resources/Material.cpp"
	"${SOURCE_DIR}/Resources/Mesh.cpp"
	"${SOURCE_DIR}/Resources/MeshGeometry.cpp"
	"${SOURCE_DIR}/Resources/Shader.cpp"
	"${SOURCE_DIR}/Resources/Texture.cpp"

	"${SOURCE_DIR}/Scene/Camera.cpp"
	"${SOURCE_DIR}/Scene/CameraEntity.cpp"
	"${SOURCE_DIR}/Scene/Entity.cpp"
	"${SOURCE_DIR}/Scene/MeshEntity.cpp"
	"${SOURCE_DIR}/Scene/Transform.cpp"

	"${SOURCE_DIR}/UI/Widget.cpp"
)

add_definitions(
  -D_SILENCE_ALL_CXX17_DEPRECATION_WARNINGS
  -D_CRT_SECURE_NO_WARNINGS
  -DSFML_STATIC
  -DGLEW_STATIC
 )

add_library(engine STATIC ${SOURCES})
target_include_directories(engine PUBLIC ${ENGINE_INCLUDE_DIR} ${THIRDPARTY_INCLUDE_DIR} PRIVATE ${SOURCE_DIR})

target_compile_features(engine PUBLIC cxx_std_17)
set_target_properties(engine PROPERTIES CXX_EXTENSIONS OFF)
