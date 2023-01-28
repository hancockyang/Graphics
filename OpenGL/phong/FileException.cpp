#pragma once
#include "FileException.hpp"

FileException::FileException(std::string filename, FileErrorID errorid)
    : m_filename{filename}, m_errorID{errorid},
      m_currentfolder{std::filesystem::current_path()} {};

std::filesystem::path const &FileException::GetCurrentFolder() const {
  return m_currentfolder;
}

std::string const &FileException::GetFileName() const { return m_filename; }

FileErrorID const &FileException::GetErrorID() const { return m_errorID; }
