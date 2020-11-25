#include "Curl.hpp"
#include "Github.hpp"
#include "VkAPI.hpp"

extern template class nlohmann::basic_json<>;

using nlohmann::json;
using bot::command::GithubCommand;
using namespace bot::cURL;

struct GithubCommand::non_alpha
{
  inline bool operator()(char c) noexcept {
    return not(isdigit(c) || isalpha(c));
  }
};

bool GithubCommand::is_latin(const std::string& text)
{
  return find_if(text.begin(), text.end(), non_alpha()) == text.end();
}

std::string GithubCommand::searchRepos(uint8_t reposCount, const std::string& user, const std::string& repo)
{
  json repos =
    json::parse(request("https://api.github.com/users/" + user + "/repos", {}));
  if (repos.is_object()) {
    return "Упс, кажется такого юзера нет.";
  }
  bool found = false;
  std::string user_repos = "Информация о репозиториях:\n";
  for (uint8_t i = 0; i < repos.size(); i++) {
    if (i == reposCount && found) {
      break;
    }
    if (repos[i]["name"].get<std::string>() == repo) {
      found = true;
    }
    user_repos += repos[i]["name"];
    user_repos += ":\n";
    user_repos += "Описание: ";
    user_repos += not repos[i]["description"].is_null() ? repos[i]["description"] : "отсутствует";
    user_repos += "\nЗвёзд: ";
    user_repos += std::to_string(repos[i]["stargazers_count"].get<long>());
    user_repos += "\nДоминирующий язык: ";
    user_repos += not repos[i]["language"].is_null() ? repos[i]["language"] : "отсутствует";
    user_repos += "\nСсылка: ";
    user_repos += repos[i]["html_url"];
    user_repos += "\nЕсли хочешь склонить: ";
    user_repos += repos[i]["clone_url"];
    user_repos += "\nФорк: ";
    user_repos += repos[i]["fork"].get<bool>() ? "да\n\n" : "нет\n\n";
  }
  return user_repos;
}

std::string GithubCommand::getUserRepo(const std::string& user, const std::string& repo)
{
  return searchRepos(1, user, repo);
}

std::string GithubCommand::getUserRepos(const std::string& user)
{
  return searchRepos(10, user);
}

std::string GithubCommand::description() const
{
  return "показать различную инфу из GitHub";
}

std::string GithubCommand::trigger() const
{
  return "+гитхаб";
}

std::string GithubCommand::execute(const CommandParams& inputData)
{
  //... Я обосрался, сделав в CommandParams поле, хранящее сообщение строкой а не вектором строк.
  return "";
}
