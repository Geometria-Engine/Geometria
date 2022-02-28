#include <string>
#include <experimental/filesystem>
#include <vector>
struct CompilerValue
{
	std::string name, value;
	int file;
};

class Compiler
{
public:
	std::string command;
	std::experimental::filesystem::path defaultProjectPath = std::experimental::filesystem::current_path().parent_path();
	std::experimental::filesystem::path resourcesPath = std::experimental::filesystem::current_path();
	std::experimental::filesystem::path resultPath;

	std::vector<std::string> majorFilePaths;
	std::vector<std::pair<std::string, std::string>> customValues;

	std::vector<CompilerValue> compValues;

	Compiler()
	{
	}
	Compiler(std::string path);

	void Compile();

	void AddImportantFilePath(std::string path);
	void AddCustomValue(std::string name, std::string value);
	void AddPropertyChange(std::string name, int file, std::string value);

	std::string ModifyCommandLine(std::string c);
};