#include "SearchName.hpp"

#include <filesystem>
#include <iostream>
#include <string>

#include "util/FileManager.hpp"
#define OUTPUT_FILE "/workspaces/data/file"
namespace user {

#pragma clang optimize off
void SearchFileByName(int iterations, const std::string& name) {
//   std::cout << "Running search-name on file '" << name << "' " << iterations << " times...\n";
  util::FileManager::ClearFile(OUTPUT_FILE);
  util::FileManager::WriteLine(OUTPUT_FILE, "Running search-name on file '" + name + "' " + std::to_string(iterations) + " times...");

  for (int i = 0; i < iterations; ++i) {
    util::FileManager::WriteLine(
        OUTPUT_FILE,
        "Iteration " + std::to_string(i + 1) + ": Searching for file \"" + name + "\"..."
    );
    bool found = false;

    for (const auto& entry : std::filesystem::recursive_directory_iterator(".")) {
      if (entry.is_regular_file() && entry.path().filename() == name) {
        util::FileManager::WriteLine(OUTPUT_FILE, "Found: " + entry.path().string());
        found = true;
      }
    }

    if (!found) {
    //   std::cout << "File \"" << name << "\" not found.\n";
      util::FileManager::WriteLine(OUTPUT_FILE, "File \"" + name + "\" not found.");
    }
  }
  util::FileManager::WriteLine(OUTPUT_FILE, "search-name operation completed.");
}
#pragma clang optimize on

}  // namespace user