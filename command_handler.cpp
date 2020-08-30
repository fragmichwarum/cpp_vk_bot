#include "command_handler.hpp"

void Command_Handler::if_empty_query(std::string peer_id) {
    query_vector answer_query = {std::make_pair("message", "задана пустая строка"),
                                 std::make_pair("peer_id", peer_id)};
    std::string query = cURL::generate_query("https://api.vk.com/method/messages.send?", answer_query);
    cURL::request(query);
}

#ifdef  CRC32_COM
    void Command_Handler::crc32_command() {
        string_vector tokenized_message = Tokenizer::tokenize_line(_message);
        if (tokenized_message[0] == "+crc32") {
            if (_message == "+crc32") {
                Command_Handler::if_empty_query(_peer_id);
            } else {
                unsigned long crc32 = CRC32::crc32gen(_message.substr
                                                      (tokenized_message[0].size()+1,
                                                      _message.size()-1).c_str());
                std::stringstream stream;
                stream << "0x" << std::hex << crc32;
                std::string crc32_str(stream.str());
                query_vector answer_query = {std::make_pair("message", crc32_str),
                                             std::make_pair("peer_id", _peer_id)};
                std::string query = cURL::generate_query("https://api.vk.com/method/messages.send?", answer_query);
                cURL::request(query);
            }
        }
    }
#endif

#ifdef  SHA256_COM
    void Command_Handler::sha256_command() {
        string_vector tokenized_message = Tokenizer::tokenize_line(_message);
        if (tokenized_message[0] == "+sha256") {
            if (_message == "+sha256") {
                if_empty_query(_peer_id);
            } else {
                SHA256 sha(_message.substr(tokenized_message[0].size()+1, _message.size()-1).c_str());
                query_vector answer_query = {std::make_pair("message", sha.hash()),
                                             std::make_pair("peer_id", _peer_id)};
                std::string query = cURL::generate_query("https://api.vk.com/method/messages.send?", answer_query);
                cURL::request(query);
            }
        }
    }
#endif

#ifdef  PIC_COM
    void Command_Handler::picture_command() {
        string_vector tokenized_message = Tokenizer::tokenize_line(_message);
        if (tokenized_message[0] == "+пикча") {
            if (_message == "+пикча") {
                std::cout << tokenized_message[0] << std::endl;
                if_empty_query(_peer_id);
            } else {
                std::string message_to_search = _message.substr(tokenized_message[0].size()+1, _message.size()-1).c_str();
                std::string answer = cURL::request("https://api.vk.com/method/photos.search?q="
                                                  + message_to_search + "&access_token=" + user_token + "&v=" + api_version);
                auto answer_parsed = nlohmann::json::parse(answer);
                if (!answer_parsed["response"]["items"][0].is_null()) {
                    long owner_id = answer_parsed["response"]["items"][0]["owner_id"];
                    long id = answer_parsed["response"]["items"][0]["id"];
                    if (owner_id != 0 && id != 0) {
                    std::string owner_id_str = std::to_string(owner_id);
                    std::string id_str = std::to_string(id);
                        query_vector picture_message = {std::make_pair("attachment", "photo" + owner_id_str + "_" + id_str),
                                                        std::make_pair("peer_id", _peer_id)};
                        std::string query = cURL::generate_query("https://api.vk.com/method/messages.send?", picture_message);
                        cURL::request(query);
                    }
                } else {
                    query_vector picture_message = {std::make_pair("message", "Не найдено фотографий!"),
                                                    std::make_pair("peer_id", _peer_id)};
                    std::string query = cURL::generate_query("https://api.vk.com/method/messages.send?", picture_message);
                    cURL::request(query);
                }
            }
        }
    }
#endif

    void Command_Handler::init_commands() {
    #ifdef  CRC32_COM
        crc32_command();
    #endif

    #ifdef SHA256_COM
        sha256_command();
    #endif

    #ifdef  PIC_COM
        picture_command();
    #endif
    }


