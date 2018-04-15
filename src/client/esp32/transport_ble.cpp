#include "../log.h"
#include "../transport.h"

std::unique_ptr<Transport>
Transport::Create(std::vector<std::string> topics,
                  std::function<void(char*, uint8_t*, unsigned int)> callback)
{
    return nullptr;
}
