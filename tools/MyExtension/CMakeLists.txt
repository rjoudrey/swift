add_swift_library(MyExtension STATIC
  MyExtension.cpp
  LINK_LIBRARIES swiftBasic swiftAST swiftFrontend
)
