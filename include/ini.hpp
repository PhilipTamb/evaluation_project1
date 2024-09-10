#include <string>
#include <unordered_map>
#include <vector>
// unsigned short load_resurce(const std::string &path);

using namespace std;

class Ini
{
public:
    const char *filename;
    vector<long> positions; //  vector of log entry positions. The i-th entry corresponds to the last byte position + 1 in the original file
    int fd;
    off_t filesize;
    std::vector<long> local_pos;
    // std::mutex m;
    char *mmap_obj;
    Ini() {};
    ~Ini() {};
    // std::vector<char*> mmap_objs;
    unsigned short load_resurce(const std::string &path);
    unsigned short get_value(const std::string &key, std::string &value);
    unsigned short set_value(const std::string &key, std::string &value);

private:

    std::unordered_map<string, string> data;
};