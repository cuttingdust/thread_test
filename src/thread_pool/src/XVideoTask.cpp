#include "XVideoTask.h"

#include <sstream>

auto XVideoTask::run() -> int
{
    /// ffmpeg -y -i test.mp4 -s 400x300 400.mp4 >log.txt 2>&1
    std::stringstream ss;
    ss << "assert\\ffmpeg.exe -y -i " << in_path << " ";
    if (width > 0 && height > 0)
        ss << " -s " << width << "x" << height << " ";
    ss << out_path;
    ss << " >" << std::this_thread::get_id() << ".txt 2>&1";
    return std::system(ss.str().c_str());
}
