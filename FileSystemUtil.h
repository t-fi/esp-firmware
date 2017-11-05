#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include <Arduino.h>

class FileSystemUtil {
public:
  static String read(String path);
  static void write(String path, String content);
  static void format();
private:
  FileSystemUtil() {}
};

#endif // FILESYSTEM_H


