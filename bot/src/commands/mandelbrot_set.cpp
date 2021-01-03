#include <complex>
#include <fstream>
#include <random>
#include <ctime>

#include "lib/include/methods/photos.hpp"
#include "bot/include/commands/mandelbrot_set.hpp"

/*constexpr*/ std::int16_t mandelbrot_point(std::int16_t size, std::int16_t x, std::int16_t y)
{
  constexpr
  std::int8_t intensivity = 100;
  std::int8_t iterations  =   0;

  std::complex<double> z(0, 0);
  std::complex<double> point(
    static_cast<double>(x) / size - 1.55,
    static_cast<double>(y) / size - 0.5
  );

  while (abs(z) < 2 && iterations++ <= intensivity)
  {
    z = z * z + point;
  }

  return iterations < intensivity
    ? (255 * iterations) / 40
    : 20;
}

std::int8_t mandelbrot_gen(std::string_view path)
{
  std::ofstream img(path.data());

  if (not img.is_open()) return -1;

  constexpr std::int16_t size = 768;

  const std::string png_size   = std::to_string(size) + " " + std::to_string(size);
  const std::string png_header = "P6\n" + png_size + " 255\n";

  img << png_header;

  std::srand(std::time(nullptr));
  const std::int16_t coefficient_r = std::rand() % 3 + 1;
  const std::int16_t coefficient_g = std::rand() % 3 + 1;
  const std::int16_t coefficient_b = std::rand() % 3 + 1;

  for (std::int16_t i = 0; i < size; ++i)
  {
    for (std::int16_t j = 0; j < size; ++j)
    {
      const std::int16_t value = mandelbrot_point(size, i, j);
      img << static_cast<unsigned char>(value / coefficient_r)
          << static_cast<unsigned char>(value / coefficient_g)
          << static_cast<unsigned char>(value / coefficient_b);
    }
  }
  img.close();
  return 0;
}

bot::command::mandelbrot_set::mandelbrot_set()
  : photos(std::make_unique<vk::photos>())
{ }

bot::command::mandelbrot_set::~mandelbrot_set() = default;

void bot::command::mandelbrot_set::execute(const vk::event::message_new& event) const
{
  mandelbrot_gen("mandelbrot.png");
  // Лютый костыль по причине того, что ВК скотина не хочет жрать мою PNG'шку.
  system("convert mandelbrot.png mandelbrot.jpg");

  std::string raw_json = photos->get_messages_upload_server(event.peer_id());

  auto photo = photos->save_messages_photo("mandelbrot.jpg", raw_json);

  messages.send(event.peer_id(), "", { photo });
}
