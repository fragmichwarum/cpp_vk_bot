#include "Curl.hpp"
#include "Github.hpp"

using std::string;
using std::to_string;
using std::find_if;
using nlohmann::json;

using bot::command::GithubCommand;
using namespace bot::cURL;

struct GithubCommand::non_alpha
{
  inline bool operator()(char c) noexcept {
    return not(isdigit(c) || isalpha(c));
  }
};

bool GithubCommand::is_latin(const string& text)
{
  return find_if(text.begin(), text.end(), non_alpha()) == text.end();
}

string GithubCommand::searchRepos(uint8_t reposCount, const string& user, const string& repo)
{
  json repos =
    json::parse(request("https://api.github.com/users/" + user + "/repos", {}));
  if (repos.is_object()) {
    return "Упс, кажется такого юзера нет.";
  }
  bool found = false;
  string user_repos = "Информация о репозиториях:\n";
  for (uint8_t i = 0; i < repos.size(); i++) {
    if (i == reposCount && found) {
      break;
    }
    if (repos[i]["name"].get<string>() == repo) {
      found = true;
    }
    user_repos += repos[i]["name"];
    user_repos += ":\n";
    user_repos += "Описание: ";
    user_repos += not repos[i]["description"].is_null() ? repos[i]["description"] : "отсутствует";
    user_repos += "\nЗвёзд: ";
    user_repos += to_string(repos[i]["stargazers_count"].get<long>());
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

string GithubCommand::getUserRepo(const string& user, const string& repo)
{
  return searchRepos(1, user, repo);
}

string GithubCommand::getUserRepos(const string& user)
{
  return searchRepos(10, user);
}

string GithubCommand::description() const
{
  return "показать различную инфу из GitHub";
}

string GithubCommand::trigger() const
{
  return "+гитхаб";
}

string GithubCommand::execute(const CommandParams& inputData)
{
  //... Я обосрался, сделав в CommandParams поле, хранящее сообщение строкой а не вектором строк.
  return "";
}
