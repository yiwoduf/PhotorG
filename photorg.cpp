/**
 * Jaeyol (Peter) Lee
 * 
 * @yiwoduf
 * 
 * yiwoduf@gmail.com
 * 
 * PhotorG - Photo Organizaer
 */
#include <iostream>
#include <filesystem>
#include <vector>
#include <algorithm>
#include <ctime>
#include <chrono>

namespace fs = std::filesystem;

int main() {
    std::string source_folder;
    std::cout << "Enter the source folder path: ";
    std::getline(std::cin, source_folder);

    // READ DIRECTORY
    std::vector<fs::path> files;
    for (const auto& entry : fs::directory_iterator(source_folder)) {
        if (fs::is_regular_file(entry)) {
            files.push_back(entry.path());
        }
    }

    // FILE EXTRACT CREATED DATE AND INFO
    std::vector<std::pair<fs::path, std::time_t>> file_times;
    for (const auto& file : files) {
        auto ftime = fs::last_write_time(file);
        auto sctime = std::chrono::system_clock::now() + 
                    std::chrono::duration_cast<std::chrono::seconds>(ftime.time_since_epoch() - fs::file_time_type::clock::now().time_since_epoch());
        std::time_t creation_time = std::chrono::system_clock::to_time_t(sctime);
        file_times.emplace_back(file, creation_time);
    }



    // CREATE FOLDER BY DATE
    std::sort(file_times.begin(), file_times.end(),
              [](const auto& a, const auto& b) { return a.second < b.second; });
    for (const auto& [file, creation_time] : file_times) {
        std::tm* local_time = std::localtime(&creation_time);
        std::string folder_name = std::to_string(local_time->tm_year + 1900) + "-" +
                                  std::to_string(local_time->tm_mon + 1) + "-" +
                                  std::to_string(local_time->tm_mday);
        fs::path new_folder = fs::path(source_folder) / folder_name;
        if (!fs::exists(new_folder)) {
            fs::create_directory(new_folder);
        }

        // MOVE FILES TO DIRECTORY
        fs::path new_file_path = new_folder / file.filename();
        fs::rename(file, new_file_path);
        std::cout << "Moved " << file.filename() << " to " << new_folder << std::endl;
    }

    return 0;
}
