#include "XMsgServer.h"

#include <iostream>
#include <sstream>

int main(int argc, char *argv[])
{
    std::cout << "main thread ID " << std::this_thread::get_id() << std::endl;
    {
        XMsgServer server;
        server.start();
        for (int i = 0; i < 10; ++i)
        {
            std::stringstream ss;
            ss << " msg : " << i + 1;
            server.sendMsg(ss.str());
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }
        server.stop();
    }
    getchar();

    return 0;
}
