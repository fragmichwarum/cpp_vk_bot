#ifndef COMMAND_HANDLER_HPP
#define COMMAND_HANDLER_HPP

#include "curl.hpp"
#include "tokenizer.hpp"

class Command_Handler {
public:
    Command_Handler(std::string message, std::string peer_id) {
        _message = message;
        _peer_id = peer_id;
    }
#define CRC32_COM
#ifdef  CRC32_COM
    void crc32_command();
#endif

#define SHA256_COM
#ifdef  SHA256_COM
    void sha256_command();
#endif

#define PIC_COM
#ifdef  PIC_COM
    void picture_command();
#endif
    void init_commands();
private:
    static void if_empty_query(std::string peer_id);
    std::string _message;
    std::string _peer_id;
};

#endif // COMMAND_HANDLER_HPP
