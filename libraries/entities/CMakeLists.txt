set(TARGET_NAME entities)

# use setup_hifi_library macro to setup our project and link appropriate Qt modules
setup_hifi_library(Network Script)

add_dependency_external_project(glm)
find_package(GLM REQUIRED)
target_include_directories(${TARGET_NAME} PUBLIC ${GLM_INCLUDE_DIRS})

include_bullet()

link_hifi_libraries(avatars shared octree gpu model fbx networking animation)

# call macro to include our dependency includes and bubble them up via a property on our target
include_dependency_includes()
