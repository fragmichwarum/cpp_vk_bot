#include <random>

#include "../bot/include/3rd_party/string_utils.hpp"
#include "../bot/include/commands/who.hpp"


template <class Iterator, class Random_generator>
static Iterator select_random(Iterator&& start, Iterator&& end, Random_generator&& generator)
{
  std::uniform_int_distribution<> dis(0, std::distance(start, end) - 1);
  std::advance(start, dis(generator));
  return start;
}

template <class Iterator>
static Iterator select_random(Iterator&& start, Iterator&& end)
{
  static std::random_device random_device;
  static std::mt19937 generator(random_device());
  return select_random(start, end, generator);
}

void bot::command::who::execute(const vk::event::message_new& event) const
{
  vk::conversation_member_list members = messages.get_conversation_members(event.peer_id());

  auto random_member = select_random(members.begin(), members.end());

  std::string random_member_id = "@id" + std::to_string(random_member->id);
  std::string random_member_name = random_member->first_name + ' ' + random_member->last_name;

  std::string user_text = third_party::string_utils::cut_first(event.text());

  (user_text.empty())
    ? messages.send(event.peer_id(), "Хмм, я думаю, что это " + random_member_id + "(" + random_member_name + ") ")
    : messages.send(event.peer_id(), "Хмм, я думаю, что " + random_member_id + "(" + random_member_name + ") " + user_text);
}
