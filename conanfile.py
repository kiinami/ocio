from conan import ConanFile
from conan.tools.cmake import CMakeToolchain, CMake, cmake_layout, CMakeDeps
from conan.tools.build import check_min_cppstd
from conan.tools.scm import Version
from conan.errors import ConanInvalidConfiguration

import os

required_conan_version='>=2.28'

class OcioOpenglConan(ConanFile):
    name = "ocio-opengl"
    version = "0.1.0"
    license = "MIT"
    author = "kinami"
    settings = "os", "compiler", "build_type", "arch"

    def requirements(self):
        self.requires("glad/2.0.8")
        self.requires("glfw/3.4")

    def layout(self):
        self.folders.source = "src"
        self.folders.build = "cmake-build-" + str(self.settings.build_type).lower()
        self.folders.generators = os.path.join(self.folders.build, "generators")

    def generate(self):
        tc = CMakeToolchain(self)
        tc.user_presets_path = False
        tc.generate()

        deps = CMakeDeps(self)
        deps.generate()

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def package(self):
        cmake = CMake(self)
        cmake.install()
