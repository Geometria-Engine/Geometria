#include "Files.h"
#include "Image/lodepng.h"
#include "Scene/SceneFile.h"
#include "nlohmann/json.hpp"
#include <experimental/filesystem>
#include <cstdio>
#include <memory>
#include "String/StringAPI.h"
#include <vector>
#include "DebugTools/DebugTools.h"

#ifdef _WIN32
#include <Windows.h>
#include <direct.h>
#define POPEN _popen
#define PCLOSE _pclose
#endif
#include <Application/Application.h>

#ifdef __linux__
#define POPEN popen
#define PCLOSE pclose
#endif

#undef CreateDirectory

std::string Files::Read(const char* url)
{
    return Files::Read(url, false);
}

std::string Files::Read(const char* url, bool isBinary)
{
    std::ifstream f;

    if (isBinary)
        f = std::ifstream(url, std::ios::binary);
    else
        f = std::ifstream(url);

    std::string content;

    std::ostringstream ss;
    ss << f.rdbuf();
    content = ss.str();

    return content;
}

std::vector<std::string> Files::ReadAndGetLines(const char* url)
{
    std::vector<std::string> allLines;

    std::ifstream file(url);
    if (file.is_open())
    {
        std::string line;
        while (std::getline(file, line))
        {
            allLines.push_back(line);
        }
    }

    file.close();

    return allLines;
}

std::string Files::Write(const char* url, std::string content)
{
    std::ofstream file(url);
    file << content;
    file.close();

    return content;
}

std::string Files::Replace(const char* oldFile, const char* newFile)
{
    return Files::Replace(oldFile, newFile, false);
}

std::string Files::Replace(const char* oldFile, const char* newFile, bool isBinary)
{
    std::string oldFileContent = Files::Read(oldFile, isBinary);
    std::string newFileContent = Files::Read(newFile, isBinary);

    //std::cout << newFileContent << std::endl;

    //Files::Remove(oldFile);
    return Files::Write(oldFile, newFileContent);
}

std::string Files::OpenImage(const char* url, int& width, int& height)
{
    std::vector<unsigned char> png;
    std::vector<unsigned char> image;
    lodepng::State setState;
    unsigned int w = 0, h = 0;
    
    unsigned state = lodepng::load_file(png, url);
    if (!state)
    {
        state = lodepng::decode(image, w, h, setState, png);
    }

    width = w;
    height = h;

    return std::string(image.begin(), image.end());
}

void Files::OpenProgram(const char* url)
{
    std::string getUrl = url;
    Files::OpenProgram(std::wstring_convert<std::codecvt_utf8<wchar_t>>().from_bytes(getUrl).c_str());
}

void Files::OpenProgram(const wchar_t* url)
{
#ifdef _WIN32
    LPCTSTR lpApplicationName = url;
    STARTUPINFO si;
    PROCESS_INFORMATION pi;

    // set the size of the structures
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    // start the program up
    CreateProcess(lpApplicationName,   // the path
        NULL,        // Command line
        NULL,           // Process handle not inheritable
        NULL,           // Thread handle not inheritable
        FALSE,          // Set handle inheritance to FALSE
        0,              // No creation flags
        NULL,           // Use parent's environment block
        NULL,           // Use parent's starting directory 
        &si,            // Pointer to STARTUPINFO structure
        &pi             // Pointer to PROCESS_INFORMATION structure (removed extra parentheses)
    );
    // Close process and thread handles. 
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
#endif
}

bool Files::Remove(const char* url)
{
    std::remove(url);

    bool failed = !std::ifstream(url);
    //if (failed) { std::perror("Error opening deleted file"); }
    return failed;
}

std::vector<unsigned char> Files::GetImageData(const char* url, int& width, int& height)
{
    std::vector<unsigned char> png;
    std::vector<unsigned char> image;
    lodepng::State setState;
    unsigned int w = 0, h = 0;

    unsigned state = lodepng::load_file(png, url);
    if (!state)
    {
        state = lodepng::decode(image, w, h, setState, png);
    }

    width = w;
    height = h;

    return image;
}

bool Files::SaveImage(std::string output, std::vector<unsigned char> data, int width, int height)
{
    unsigned error = lodepng::encode(output, data, width, height);
    return error;
}

bool Files::LoadScene(std::string file)
{
    

    return true;
}

void Files::CreateDirectory(const char* url)
{
    std::experimental::filesystem::create_directories(url);
}

void Files::CopyDirectory(const char* url, const char* dest)
{
    DeleteDirectory(dest);
    std::experimental::filesystem::copy(url, dest, std::experimental::filesystem::copy_options::recursive);
}

void Files::DeleteDirectory(const char* url)
{
    std::experimental::filesystem::remove_all(url);
}

bool Files::DirectoryExists(const char* url)
{
    std::experimental::filesystem::path urlPath(url);
    return std::experimental::filesystem::exists(urlPath);
}

std::string Files::GetGamePath()
{
    return Files::GetDirectoryOf(Files::GetExecutablePath().c_str()) + "/Game";
}

int Files::UnZIP(const char* zipUrl)
{
    unzFile zipfile = unzOpen(zipUrl);
    if (zipfile == NULL)
    {
        printf("%s: not found\n");
        return -1;
    }

    // Get info about the zip file
    unz_global_info global_info;
    if (unzGetGlobalInfo(zipfile, &global_info) != UNZ_OK)
    {
        printf("could not read file global info\n");
        unzClose(zipfile);
        return -1;
    }

    // Buffer to hold data read from the zip file.
    char read_buffer[8192];

    // Loop to extract all files
    uLong i;
    for (i = 0; i < global_info.number_entry; ++i)
    {
        // Get info about current file.
        unz_file_info file_info;
        char filename[512];
        if (unzGetCurrentFileInfo(
            zipfile,
            &file_info,
            filename,
            512,
            NULL, 0, NULL, 0) != UNZ_OK)
        {
            printf("could not read file info\n");
            unzClose(zipfile);
            return -1;
        }

        std::string fullUrl = Files::GetDirectoryOf(zipUrl);
        fullUrl += "/";
        fullUrl += filename;

        // Check if this entry is a directory or file.
        const size_t filename_length = strlen(filename);
        if (filename[filename_length - 1] == '/')
        {
            // Entry is a directory, so create it.
            //printf("dir:%s\n", filename);
            Files::CreateDirectory(fullUrl.c_str());
        }
        else
        {
            // Entry is a file, so extract it.
            //printf("file:%s\n", filename);
            if (unzOpenCurrentFile(zipfile) != UNZ_OK)
            {
                printf("could not open file\n");
                unzClose(zipfile);
                return -1;
            }

            // Open a file to write out the data.
            FILE* out = fopen(fullUrl.c_str(), "wb");
            if (out == NULL)
            {
                printf("could not open destination file\n");
                unzCloseCurrentFile(zipfile);
                unzClose(zipfile);
                return -1;
            }

            int error = UNZ_OK;
            do
            {
                error = unzReadCurrentFile(zipfile, read_buffer, 8192);
                if (error < 0)
                {
                    printf("error %d\n", error);
                    unzCloseCurrentFile(zipfile);
                    unzClose(zipfile);
                    return -1;
                }

                // Write data to file.
                if (error > 0)
                {
                    fwrite(read_buffer, error, 1, out); // You should check return of fwrite...
                }
            } while (error > 0);

            fclose(out);
        }

        unzCloseCurrentFile(zipfile);

        // Go the the next entry listed in the zip file.
        if ((i + 1) < global_info.number_entry)
        {
            if (unzGoToNextFile(zipfile) != UNZ_OK)
            {
                printf("cound not read next file\n");
                unzClose(zipfile);
                return -1;
            }
        }
    }

    unzClose(zipfile);

    return 0;
}

std::string Files::GetDirectoryOf(const char* file)
{
    std::string fileToString = file;
    size_t found = fileToString.find_last_of("/\\");
    return(fileToString.substr(0, found));
}

std::string Files::WhereIs(std::string name)
{
    std::string result;
    std::string command;

#ifdef _WIN32
    command = "where " + name;
#endif

#ifdef __linux__
    command = "which " + name;
#endif

    result = Files::RunCommand(command);
    result = StringAPI::RemoveAll(result, "\n");

    std::ifstream test(result);
    if (!test)
    {
        std::cout << "The file \"" << command << "\" doesn't exist" << std::endl;
        return std::string();
    }

    return result;
}

std::string Files::RunCommand(std::string cmd)
{
    std::string result;
    char buffer[128];
    FILE* pipe = POPEN(cmd.c_str(), "r");
    if (!pipe) return std::string();
    try {
        while (fgets(buffer, sizeof buffer, pipe) != NULL) {
            result += buffer;
        }
    }
    catch (...) {
        PCLOSE(pipe);
    }
    PCLOSE(pipe);

    return result;
}

std::string Files::GetPathFromCommand(std::string cmd)
{
    std::string result;
    result = Files::RunCommand(cmd);
    result = StringAPI::RemoveAll(result, "\n");

    std::ifstream test(result);
    if (!test)
    {
        std::cout << "The path from \"" << cmd << "\" doesn't exist" << std::endl;
        return std::string();
    }

    return result;
}

std::string Files::GetValueFromCommand(std::string cmd)
{
    std::string result;
    result = Files::RunCommand(cmd);
    result = StringAPI::RemoveAll(result, "\n");
    return result;
}

void Files::ClearConsole()
{
    if (Application::IsPlatform(Application::Windows))
        system("cls");
    else if (Application::IsPlatform(Application::Linux))
        system("clear");
}

std::string Files::ConvertToWindowsCmdPath(std::string path)
{
    std::string l, finalResult;
    path = StringAPI::ReplaceAll(path, "\\", "/");
    std::istringstream cStream(path);
    bool isFirst = true;
    while (std::getline(cStream, l, '/'))
    {
        std::string changedLine;
        if (l.find(" ") != std::string::npos)
        {
            if (!isFirst)
                changedLine += "/";

            changedLine += "\"";
            changedLine += l;
            changedLine += "\"";
        }
        else
        {
            if (!isFirst)
                changedLine += "/";

            changedLine += l;
        }

        isFirst = false;
        finalResult += changedLine;
    }

    return finalResult;
}

std::string Files::GetExecutablePath()
{
    std::string path;

    // TODO: Add Support For Linux

#ifdef _WIN32
    char buffer[MAX_PATH];
    GetModuleFileNameA(NULL, buffer, MAX_PATH);
    path = std::string(buffer);
#endif

    return path;
}

std::vector<std::string> Files::OpenTexturePack(const char* gtxp)
{
    std::string txpContent = Files::Read(gtxp, true);
    auto jsonBlocks = nlohmann::json::parse(txpContent);
    auto entriesArray = jsonBlocks["TexturePack"];

    int total = 0;
    for (auto i : entriesArray)
    {
        std::string n = i["ID"];
        int currentNumb = std::stoi(n);

        if (total < currentNumb)
            total = currentNumb;
    }

    std::vector<std::string> texturePackLinks(total + 1);
    std::cout << texturePackLinks.size() << std::endl;

    std::string directory = Files::GetDirectoryOf(gtxp);

    for (auto i : entriesArray)
    {
        std::string n = i["ID"];
        int numb = std::stoi(n);
        std::string imgName = i["Image"];

        texturePackLinks[numb] += directory;
        texturePackLinks[numb] += "/";
        texturePackLinks[numb] += imgName;
        texturePackLinks[numb] += ".png";
    }

    return texturePackLinks;
}

void Files::ChangeCurrentDirectory(std::string path)
{
    std::string finalDirectory;

    std::string pathWithFileSystem = std::experimental::filesystem::current_path().u8string() + "\\" + path;

    if(Files::DirectoryExists(pathWithFileSystem.c_str()) ||
        Files::DirectoryExists(path.c_str()))
    {
        if(Application::IsPlatform(Application::Windows))
        {
            if(path.find(":/") == std::string::npos || path.find(":\\") == std::string::npos)
            {
                finalDirectory = std::experimental::filesystem::current_path().u8string() + "\\";
            }
    
            finalDirectory += path;
    
            #ifdef _WIN32
            _chdir(finalDirectory.c_str());
            #endif
        }
        else if(Application::IsPlatform(Application::Linux))
        {
            // TODO: Make it for Linux.
        }
    }
}

std::vector<float> Files::ReadOBJ(const char* url)
{
    std::vector<std::string> lines = Files::ReadAndGetLines(url);
    std::vector<std::array<float, 4>> currentVertices, resultVertices;
    std::vector<std::array<int, 4>> currentIndices;
    std::vector<float> finalResult;
    bool cancel = false;

    for(auto i : lines)
    {
        if(!StringAPI::StartsWith(i, "vn ") && !StringAPI::StartsWith(i, "vt "))
        {
            if(StringAPI::StartsWith(i, "v "))
            {
                std::string res = StringAPI::RemoveAll(i, "v ");
                std::vector<std::string> split = StringAPI::SplitIntoVector(res, " ");
                std::array<float, 4> v;
                v[3] = 1.0f;
                for(int vert = 0; vert < split.size(); vert++)
                {
                    // Here...
                    v[vert] = std::stof(split[vert]);
                    //std::cout << "Vertex: " << v[vert] << std::endl;
                }
                //std::cout << "Vertex: " << v[3] << std::endl;
                currentVertices.push_back(v);
            }
            else if(StringAPI::StartsWith(i, "f "))
            {
                std::string res = StringAPI::RemoveAll(i, "f ");
                std::vector<std::string> split = StringAPI::SplitIntoVector(res, " ");

                std::array<int, 4> indices;
                indices[3] = -1;

                if(split.size() <= 4)
                {
                    for(int index = 0; index < split.size(); index++)
                    {
                        std::vector<std::string> coords = StringAPI::SplitIntoVector(split[index], "/");
                        indices[index] = std::stoi(coords[0]) - 1;
                        //std::cout << "Index: " << indices[index] << std::endl;
                    }
        
                    //std::cout << "indices[3] is " << indices[3] << std::endl;
                    currentIndices.push_back(indices);
                }
                else
                {
                    std::cout << "[ERROR]: Shapes in the model that aren't tries or quads are not allowed!\n";
                    std::cout << "[ERROR]: Cancelling...\n";
                    cancel = true;
                }
            }
        }
    }

    if(!cancel)
    {
        //std::cout << "Finished getting variables!" << std::endl;
    
        for(auto i : currentIndices)
        {
            int biggestIndex = 0;
            for(int count = 0; count < 4; count++)
            {
                //std::cout << "Index " << count << " is ";
                if(i[count] >= biggestIndex)
                    biggestIndex = i[count];
    
                if(i[count] == -1)
                    i[count] = biggestIndex;
    
                //std::cout << i[count] << "\n";
    
                resultVertices.push_back(currentVertices[i[count]]);
            }
        }
    
        //std::cout << "Finished setting final vertices!" << std::endl;
    
        for(auto i : resultVertices)
        {
            for(int res = 0; res < 4; res++)
            {
                finalResult.push_back(i[res]);
            }
        }
    
        //std::cout << "Done!" << std::endl;
    }

    return finalResult;
}
