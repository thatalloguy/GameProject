
file(GLOB_RECURSE EDITOR_SRC

        src/ProjectManager.h
        src/ProjectManager.cpp

        src/LakeUtils.h
)


add_executable(Quack-Editor ${EDITOR_SRC}
    main.cpp
)