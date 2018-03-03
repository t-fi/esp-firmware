#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include <Arduino.h>
#include <string>

class FileSystemUtil {
public:
  static std::string read(std::string path);
  static void write(std::string path, std::string content);
  static void format();
private:
  FileSystemUtil() {}
};

#endif // FILESYSTEM_H
