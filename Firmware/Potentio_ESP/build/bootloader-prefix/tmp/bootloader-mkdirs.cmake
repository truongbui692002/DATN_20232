# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "D:/ESP_IDF_V5_1/Espressif/frameworks/esp-idf-v5.1/components/bootloader/subproject"
  "D:/GIT/esp32-idf/Potentio_ESP/build/bootloader"
  "D:/GIT/esp32-idf/Potentio_ESP/build/bootloader-prefix"
  "D:/GIT/esp32-idf/Potentio_ESP/build/bootloader-prefix/tmp"
  "D:/GIT/esp32-idf/Potentio_ESP/build/bootloader-prefix/src/bootloader-stamp"
  "D:/GIT/esp32-idf/Potentio_ESP/build/bootloader-prefix/src"
  "D:/GIT/esp32-idf/Potentio_ESP/build/bootloader-prefix/src/bootloader-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "D:/GIT/esp32-idf/Potentio_ESP/build/bootloader-prefix/src/bootloader-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "D:/GIT/esp32-idf/Potentio_ESP/build/bootloader-prefix/src/bootloader-stamp${cfgdir}") # cfgdir has leading slash
endif()
