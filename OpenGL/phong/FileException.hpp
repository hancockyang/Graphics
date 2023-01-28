#pragma once
#include <string>
#include <filesystem>


enum class FileErrorID {
  ObjFileFailedToOpen,
  OFFFileFailedToOpen,
  ShaderFileFailedToOpen,
  AssimpReadFileError
};

class FileException {
public:
  FileException(std::string, FileErrorID);
  std::string const &GetFileName() const;
  std::filesystem::path const &GetCurrentFolder() const;
  FileErrorID const &GetErrorID() const;

private:
  std::string m_filename;
  std::filesystem::path m_currentfolder;
  FileErrorID m_errorID;
};

