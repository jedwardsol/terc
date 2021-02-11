#include <cstdlib>
#include <filesystem>
namespace fs=std::filesystem;

int main(int argc, char *argv[]) 
{
    fs::path    me{argv[0]};

    fs::path    arithmetic = me.parent_path() / "unittest_arithmetic.exe";
    fs::path    terc6      = me.parent_path() / "unittests_terc6.exe";

    system(arithmetic.string().c_str());
    system(terc6.string().c_str());
}
