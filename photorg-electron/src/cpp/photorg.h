// photorg.h
#pragma once

#include <filesystem>
#include <string>
#include <vector>

namespace fs = std::filesystem;

class Photorg {
public:
  static bool organize_photos(const std::string &source_folder, bool yearly,
                              bool monthly, bool daily);
  static std::vector<std::string> get_directory_tree(const std::string &path);

private:
  static bool is_supported_image_format(const fs::path &file_path);
};
