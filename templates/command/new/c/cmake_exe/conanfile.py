from conan import ConanFile
from conan.tools.cmake import CMake, CMakeToolchain, cmake_layout
from conan.tools.files import get, copy
import os

class {{ name | replace('-', ' ') | replace('_', ' ') | title | replace(' ', '') }}(ConanFile):
    name = "{{name}}"
    description = "{{description|default('')}}"
    license = "MIT"
    url = "https://github.com/conan-io/conan-center-index"
    homepage = "https://github.com/{{author}}/{{name}}"
    package_type = "application"
    settings = "os", "arch", "compiler", "build_type"

    def configure(self) -> None:
        self.settings.rm_safe("compiler.cppstd")
        self.settings.rm_safe("compiler.libcxx")

    def layout(self) -> None:
        cmake_layout(self, src_folder="src")

    def source(self) -> None:
        get(self, **self.conan_data["sources"][self.version], strip_root=True)

    def generate(self) -> None:
        CMakeToolchain(self).generate()

    def build(self) -> None:
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def package(self) -> None:
        copy(self, "LICENSE", self.source_folder, os.path.join(self.package_folder, "licenses"))
        CMake(self).install()

    def package_info(self) -> None:
        self.cpp_info.includedirs = []
        self.cpp_info.libdirs = []
        self.cpp_info.frameworkdirs = []
        self.buildenv_info.prepend_path("PATH", os.path.join(self.package_folder, "bin"))

