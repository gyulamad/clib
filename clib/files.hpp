#pragma once

#include <vector>
#include <filesystem>
#include <fstream>
#include <sys/stat.h>

#include "err.hpp"

using namespace std;

namespace clib {

    string file_extension_with_dot(const string& extension) {
        return extension[0] == 0 ? "" : ((extension[0] == '.' ? "" : ".") + extension);
    }

    vector<string> file_find_by_extension(const string& folder = ".", const string& extension = "") {
        vector<string> files;

        for (const auto& entry : filesystem::directory_iterator(folder)) {
            // cout << "ENTRY:" << entry.path() << endl;
            if (
                (entry.is_directory() || entry.is_regular_file()) && 
                (
                    extension.empty() || 
                    str_ends_with(
                        file_extension_with_dot(extension), 
                        entry.path().filename().string()
                    )
                )
            ) {
                // cout << "FILE:" << entry.path() << endl;
                files.push_back(entry.path().string());
            } else if (entry.is_directory()) {
                // Recursively search in subdirectories
                vector<string> subs = file_find_by_extension(entry.path(), extension);
                files.insert(files.end(), subs.begin(), subs.end());
            }
        }

        return files;
    }

    vector<string> file_find_by_extensions(const string& folder, const vector<string>& extensions) {
        vector<string> files;

        for (const string& extension : extensions) {
            vector<string> matches = file_find_by_extension(folder, extension);
            files.insert(files.end(), matches.begin(), matches.end());
        }

        return files;
    }

    string file_replace_extension(const string& filename, const string& extension) {
        // Find the position of the last dot (.) in the file name
        size_t lastDotPos = filename.find_last_of('.');

        // Check if the lastDotPos is not at the end of the filename
        if (lastDotPos != string::npos && lastDotPos != filename.length() - 1) {
            // If the extension already has a leading dot, use it as is
            // Otherwise, add a leading dot
            return filename.substr(0, lastDotPos) + file_extension_with_dot(extension);
        }

        // If there's no dot in the file name or the dot is at the end,
        // simply append the extension with a leading dot if needed
        return filename + file_extension_with_dot(extension);
    }

    string file_get_extension(const string& filename) { //  TODO: tests
        size_t lastDotPos = filename.find_last_of('.');
        
        // Check if the lastDotPos is not at the end of the filename
        if (lastDotPos != string::npos && lastDotPos != filename.length() - 1) {
            return filename.substr(lastDotPos + 1);
        }

        // If there's no dot in the file name or the dot is at the end, return an empty string
        return "";
    }

    bool file_exists(const string& filePath) {
        filesystem::path pathToCheck(filePath);
        return filesystem::exists(pathToCheck);
    }

    void file_create_path(const string& path) {
        filesystem::path pathToCreate(path);

        try {
            // Create the directory and its parent directories (if they don't exist)
            filesystem::create_directories(pathToCreate);
        } catch (const exception& e) {
            throw ERROR("Unable to create folder: " + string(e.what()));
        }
    }

    time_t file_get_mtime(const string& filePath) {
        struct stat fileInfo;
        if (stat(filePath.c_str(), &fileInfo) != 0)
            throw ERROR("Unable to get file information: " + filePath);
        
        return fileInfo.st_mtime;
    }

    string file_get_contents(const string& filename) {
        ifstream file(filename);
        if (!file.is_open())
            throw ERROR("Unable to open file for reading: " + filename);

        string content((istreambuf_iterator<char>(file)), (istreambuf_iterator<char>()));
        file.close();

        return content;
    }

    void file_put_contents(const string& filename, const string& data, bool append = false) {
        // Check if the file is a symlink
        if (filesystem::is_symlink(filename))
            throw ERROR("Symlink detected. Refusing to open: " + filename);

        // Check if the file is a special file (e.g., character or block device)
        if (filesystem::is_character_file(filename) || filesystem::is_block_file(filename))
            throw ERROR("Special file detected. Refusing to open: " + filename);
            
        ofstream file;
        // FlawFinder: ignore
        file.open(filename, append ? ios::out | ios::app : ios::out);

        if (!file.is_open())
            throw ERROR("Unable to open file for writing: " + filename);

        file << data;
        file.close();
    }

    bool is_dir(const string& path) {
        return std::filesystem::is_directory(path);
    }

    void file_copy(const string& fileFrom, const string& fileTo) {
        // Open the source file for reading
        ifstream inputFile(fileFrom, ios::binary); // ios::binary ensures correct handling of binary files

        // Check if the source file is open
        if (!inputFile.is_open())
            throw ERROR("Error opening source file: " COLOR_FILENAME + fileFrom + COLOR_DEFAULT);

        // Open the destination file for writing
        ofstream outputFile(fileTo, ios::binary);

        // Check if the destination file is open
        if (!outputFile.is_open())
            throw ERROR("Error opening destination file: " COLOR_FILENAME + fileTo + COLOR_DEFAULT);

        // Copy the contents from the source file to the destination file
        outputFile << inputFile.rdbuf();

        // Close the files
        inputFile.close();
        outputFile.close();
    }
}