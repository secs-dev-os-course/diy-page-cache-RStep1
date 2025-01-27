#pragma once

#include <string>

namespace user::util {

class FileManager {
public:
  static void WriteLine(const std::string& filename, const std::string& content, bool append = true);

  static void ClearFile(const std::string& filename);
};

}  // namespace user::util