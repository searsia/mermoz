/*
 * MIT License
 *
 * Copyright (c) 2018 Qwant Research
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 * Author:
 * Noel Martin (n.martin@qwantresearch.com)
 *
 */

#ifndef MERMOZ_ROBOTS_H__
#define MERMOZ_ROBOTS_H__

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include <curl/curl.h>

#include "common/urlparser.hpp"
#include "common/logs.hpp"

namespace mermoz
{
namespace common
{

class Robots
{
public:
  Robots (std::string host, std::string user_agent) :
    host(host), user_agent(user_agent), up_host(UrlParser(host)), crawl_delay(4)
  {
    std::ostringstream oss;

    long err;
    if ((err = fetch_robots()) != CURLE_OK)
    {
      oss << "Could not fetch robots.txt for: " << host;
      oss << " HTTP_ERROR(" << err << ")";
      print_error(oss.str());
      return;
    }

    if (!parse_file())
    {
      oss << "Could not parse robots.txt for: " << host;
      print_error(oss.str());
      return;
    }
  }

  bool is_allowed(UrlParser& up);
  bool is_allowed(std::string url);

private:
  const std::string host;
  const std::string user_agent;
  int crawl_delay; // milliseconds

  std::string robots_file;

  UrlParser up_host;
  std::vector<UrlParser> walls;
  std::vector<UrlParser> doors;

  long fetch_robots();
  static size_t write_function (char* ptr, size_t size, size_t nmemb, void* userdata);

  bool parse_file();
}; // class Robots

} // namespace common
} // namespace mermoz

#endif // MERMOZ_ROBOTS_H__