include("../../libcogra/buildutils/CreateApp.cmake")
project(ANS)
set(ContribLibraries COGRA GLM STBIMAGE)
CreateApp(ContribLibraries)
