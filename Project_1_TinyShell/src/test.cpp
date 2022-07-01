#include <string>
#include <iostream>
#include <filesystem>
namespace fs = std::experimental::filesystem;

int main()
{
    std::string path = "C:\\Users\\X1 carbon\\OneDrive - Hanoi University of Science and Technology\\20212";
    for (const auto &entry : fs::directory_iterator(path))
        std::cout << entry.path() << std::endl;
}