#include "curl.hpp"
#include "command_handler.hpp"
#include "help_autogen.hpp"

void init_bot() {
    query_vector lp_parameters = {
        std::make_pair("group_id", group_id)
    };
    std::string lp_query =
        cURL::generate_query("https://api.vk.com/method/groups.getLongPollServer?", lp_parameters);
    while (true) {
        std::string get_lp   = cURL::request(lp_query);
        auto lp_parsed       = nlohmann::json::parse(get_lp);
        std::string key      = lp_parsed["response"]["key"];
        std::string server   = lp_parsed["response"]["server"];
        long        ts_digit = lp_parsed["response"]["ts"];
        std::string ts       = std::to_string(ts_digit);

        std::string lp = cURL::request(server + "?act=a_check&key=" + key + "&ts=" + ts + "&wait=30");
        if (!lp.empty()) {
            ts_digit++;
            auto parsed = nlohmann::json::parse(lp);
            if (parsed["updates"]
                      [0]
                      ["object"]
                      ["message"]
                      ["text"] != "")
            {
                std::string received_message =
                    parsed["updates"]
                          [0]
                          ["object"]
                          ["message"]
                          ["text"];
                long peer_id_digit =
                    parsed["updates"]
                          [0]
                          ["object"]
                          ["message"]
                          ["peer_id"];
                std::string peer_id = std::to_string(peer_id_digit);

                Command_Handler handler(received_message, peer_id);
                #ifdef HELP_COM
                    help_command(received_message, peer_id);
                #endif

                #ifdef  CRC32_COM
                    handler.crc32_command();
                #endif

                #ifdef SHA256_COM
                    handler.sha256_command();
                #endif

                #ifdef  PIC_COM
                    handler.picture_command();
                #endif
            }
        }
    }
}
