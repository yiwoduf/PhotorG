

/**
 * Jaeyol (Peter) Lee
 *
 * @yiwoduf
 *
 * yiwoduf@gmail.com
 *
 * PhotorG - Photo Organizaer
 */
#include <algorithm>
#include <chrono>
#include <ctime>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <mutex>
#include <string>
#include <thread>
#include <vector>
namespace fs = std::filesystem;

/* Check File Extension Type */
bool is_supported_image_format(const fs::path &file_path) {
  std::vector<std::string> supported_extensions = {".jpg",   ".jpeg", ".jfif",
                                                   ".pjpeg", ".pjp",  ".gif",
                                                   ".png",   ".heic", ".heif"};
  std::string extension = file_path.extension().string();
  std::transform(extension.begin(), extension.end(), extension.begin(),
                 ::tolower);
  return std::find(supported_extensions.begin(), supported_extensions.end(),
                   extension) != supported_extensions.end();
}

/* Printing Directory Structure */
void print_directory_tree(const fs::path &path, const std::string &prefix = "",
                          const std::string &children_prefix = "") {
  if (fs::is_directory(path)) {
    std::cout << "\033[1;34m" << prefix << path.filename().string() << "\033[0m"
              << std::endl;
    std::vector<fs::path> entries;
    for (const auto &entry : fs::directory_iterator(path)) {
      entries.push_back(entry.path());
    }
    for (size_t i = 0; i < entries.size(); ++i) {
      if (i < 5 || i == entries.size() - 1) {
        bool is_last = i == entries.size() - 1;
        std::string new_prefix = children_prefix + (is_last ? "└── " : "├── ");
        std::string new_children_prefix =
            children_prefix + (is_last ? "    " : "│   ");
        print_directory_tree(entries[i], new_prefix, new_children_prefix);
      } else if (i == 5) {
        std::cout << children_prefix << "├── ..." << std::endl;
      }
    }
  } else {
    std::cout << prefix << path.filename().string() << std::endl;
  }
}

int main() {
  // READ PATH CONFIG
  std::string path_file = "Settings.ini";
  std::string source_folder;
  bool yearly = false, monthly = false, daily = false;
  std::ifstream path_stream(path_file);
  if (path_stream.is_open()) {
    std::string line;
    while (std::getline(path_stream, line)) {
      // Remove White Space
      line.erase(std::remove_if(line.begin(), line.end(), ::isspace),
                 line.end());

      if (line.find("YEARLY=") != std::string::npos) {
        std::string value = line.substr(line.find("=") + 1, std::string::npos);
        std::transform(value.begin(), value.end(), value.begin(), ::tolower);
        yearly = (value == "true");
      } else if (line.find("MONTHLY=") != std::string::npos) {
        std::string value = line.substr(line.find("=") + 1, std::string::npos);
        std::transform(value.begin(), value.end(), value.begin(), ::tolower);
        monthly = (value == "true");
      } else if (line.find("DAILY=") != std::string::npos) {
        std::string value = line.substr(line.find("=") + 1, std::string::npos);
        std::transform(value.begin(), value.end(), value.begin(), ::tolower);
        daily = (value == "true");
      } else if (line.find("PATH=") != std::string::npos) {
        source_folder = line.substr(line.find("=") + 1, std::string::npos);
      }
    }
    path_stream.close();
    std::cout << "\033[1m\033[32m[SUCCESS] DIRECTORY PATH FOUND!\033[0m"
              << std::endl;
  } else {
    std::cout << "[ERROR] FAILED TO OPEN DIRECTORY" << path_file << std::endl;
    return 1;
  }

  // READ DIRECTORY
  std::vector<fs::path> files;
  for (const auto &entry : fs::directory_iterator(source_folder)) {
    if (fs::is_regular_file(entry) && is_supported_image_format(entry.path())) {
      files.push_back(entry.path());
    } else {
      std::cout << "\033[1m\033[31m[SKIP] Not a picture format: "
                << entry.path() << "\033[0m" << std::endl;
    }
  }

  auto start_time = std::chrono::high_resolution_clock::now();

  // FILE EXTRACT CREATED DATE AND INFO
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

  // CREATE FOLDER BY DATE
  std::sort(file_times.begin(), file_times.end(),
            [](const auto &a, const auto &b) { return a.second < b.second; });

  // Mutex for thread-safe output
  std::mutex cout_mutex;

  // Define the number of threads to use
  const int num_threads = 4;

  // Create a vector to store the threads
  std::vector<std::thread> threads;

  size_t total_files = file_times.size();
  size_t moved_files = 0;

  // Divide the file_times vector into chunks and process them in parallel
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

        // MOVE FILES TO DIRECTORY
        fs::path new_file_path = new_folder / file.filename();
        fs::rename(file, new_file_path);

        // Lock the mutex before printing output
        std::lock_guard<std::mutex> lock(cout_mutex);
        std::cout << "\033[1m[" << ++moved_files << "/" << total_files
                  << "]\033[0m Moved [" << file.filename() << "] to ["
                  << new_folder << "]" << std::endl;
      }
    });
  }

  // Wait for all threads to finish
  for (auto &thread : threads) {
    thread.join();
  }

  print_directory_tree(source_folder);

  std::cout << "\033[1m\033[32m[COMPLETE] ALL PHOTOS ORGANIZED!\033[0m"
            << std::endl;

  auto end_time = std::chrono::high_resolution_clock::now();
  auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(
                      end_time - start_time)
                      .count();
  std::cout << "Program executed in " << duration << " milliseconds."
            << std::endl;

  return 0;
}
