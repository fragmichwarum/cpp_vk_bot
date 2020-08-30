#include "help_autogen.hpp"

#ifdef HELP_COM
void help_command(std::string message, std::string peer_id) {
    string_vector tokenized_message = Tokenizer::tokenize_line(message);
    if (tokenized_message[0] == "+помощь") {
        std::string help_info;
        help_info += "Список команд:\n";
        #ifdef CRC32_COM
            help_info += "+crc32 {...} - сгенерить CRC-32 хеш-сумму строки\n";
        #endif
        #ifdef SHA256_COM
            help_info += "+sha256 {...} - сгенерить SHA-256 хеш-сумму строки\n";
        #endif
        #ifdef PIC_COM
            help_info += "+пикча {...} - найти картинку среди просторов ВК\n";
        #endif
        query_vector answer_query = {std::make_pair("message", help_info),
                                     std::make_pair("peer_id", peer_id)};
        std::string query = cURL::generate_query("https://api.vk.com/method/messages.send?", answer_query);
        cURL::request(query);
    }
}
#endif
