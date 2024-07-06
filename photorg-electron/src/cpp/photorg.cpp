/**
 * Jaeyol (Peter) Lee
 *
 * @yiwoduf
 *
 * yiwoduf@gmail.com
 *
 * PhotorG - Photo Organizaer
 */
// photorg.cpp

#include "photorg.h"
#include <algorithm>
#include <chrono>
#include <ctime>
#include <fstream>
#include <iostream>
#include <mutex>
#include <thread>

bool Photorg::is_supported_image_format(const fs::path &file_path) {
  std::vector<std::string> supported_extensions = {".jpg",   ".jpeg", ".jfif",
                                                   ".pjpeg", ".pjp",  ".gif",
                                                   ".png",   ".heic", ".heif"};
  std::string extension = file_path.extension().string();
  std::transform(extension.begin(), extension.end(), extension.begin(),
                 ::tolower);
  return std::find(supported_extensions.begin(), supported_extensions.end(),
                   extension) != supported_extensions.end();
}

bool Photorg::organize_photos(const std::string &source_folder, bool yearly,
                              bool monthly, bool daily) {
  std::vector<fs::path> files;
  for (const auto &entry : fs::directory_iterator(source_folder)) {
    if (fs::is_regular_file(entry) && is_supported_image_format(entry.path())) {
      files.push_back(entry.path());
    }
  }

  auto start_time = std::chrono::high_resolution_clock::now();

  std::vector<std::pair<fs::path, std::time_t>> file_times;
  for (const auto &file : files) {
    auto ftime = fs::last_write_time(file);
    auto sctime = std::chrono::system_clock::now() +
                  std::chrono::duration_cast<std::chrono::seconds>(
                      ftime.time_since_epoch() -
                      fs::file_time_type::clock::now().time_since_epoch());
    std::time_t creation_time = std::chrono::system_clock::to_time_t(sctime);
    file_times.emplace_back(file, creation_time);
  }

  std::sort(file_times.begin(), file_times.end(),
            [](const auto &a, const auto &b) { return a.second < b.second; });

  std::mutex cout_mutex;
  const int num_threads = 4;
  std::vector<std::thread> threads;
  size_t total_files = file_times.size();
  size_t moved_files = 0;

  for (size_t i = 0; i < num_threads; ++i) {
    size_t start = i * (total_files / num_threads);
    size_t end = (i == num_threads - 1) ? total_files
                                        : (i + 1) * (total_files / num_threads);

    threads.emplace_back([&, start, end] {
      for (size_t j = start; j < end; ++j) {
        const auto &[file, creation_time] = file_times[j];
        std::tm *local_time = std::localtime(&creation_time);
        std::string year_folder = std::to_string(local_time->tm_year + 1900);
        std::string month_folder = std::to_string(local_time->tm_mon + 1);
        std::string day_folder = std::to_string(local_time->tm_mday);

        fs::path new_folder = fs::path(source_folder);
        if (yearly) {
          new_folder /= year_folder;
          if (!fs::exists(new_folder)) {
            fs::create_directory(new_folder);
          }
        }
        if (monthly) {
          new_folder /= year_folder + "-" + month_folder;
          if (!fs::exists(new_folder)) {
            fs::create_directory(new_folder);
          }
        }
        if (daily) {
          new_folder /= year_folder + "-" + month_folder + "-" + day_folder;
          if (!fs::exists(new_folder)) {
            fs::create_directory(new_folder);
          }
        }

        fs::path new_file_path = new_folder / file.filename();
        try {
          fs::rename(file, new_file_path);
        } catch (const fs::filesystem_error &e) {
          std::cerr << "Error moving file: " << e.what() << std::endl;
        }

        std::lock_guard<std::mutex> lock(cout_mutex);
        ++moved_files;
      }
    });
  }

  for (auto &thread : threads) {
    thread.join();
  }

  auto end_time = std::chrono::high_resolution_clock::now();
  auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(
                      end_time - start_time)
                      .count();

  std::cout << "Photo organization completed in " << duration
            << " milliseconds." << std::endl;

  return (moved_files == total_files);
}

std::vector<std::string> Photorg::get_directory_tree(const std::string &path) {
  std::vector<std::string> result;

  std::function<void(const fs::path &, const std::string &,
                     const std::string &)>
      tree_helper;
  tree_helper = [&result, &tree_helper](
                    const fs::path &path, const std::string &prefix = "",
                    const std::string &children_prefix = "") {
    if (fs::is_directory(path)) {
      result.push_back(prefix + path.filename().string());
      std::vector<fs::path> entries;
      for (const auto &entry : fs::directory_iterator(path)) {
        entries.push_back(entry.path());
      }
      std::sort(entries.begin(), entries.end());
      for (size_t i = 0; i < entries.size(); ++i) {
        if (i < 5 || i == entries.size() - 1) {
          bool is_last = i == entries.size() - 1;
          std::string new_prefix =
              children_prefix + (is_last ? "└── " : "├── ");
          std::string new_children_prefix =
              children_prefix + (is_last ? "    " : "│   ");
          tree_helper(entries[i], new_prefix, new_children_prefix);
        } else if (i == 5) {
          result.push_back(children_prefix + "├── ...");
        }
      }
    } else {
      result.push_back(prefix + path.filename().string());
    }
  };

  tree_helper(fs::path(path), "", "");
  return result;
}
