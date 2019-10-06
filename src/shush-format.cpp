#include "shush-format.hpp"
#include <algorithm>

bool shush::format::
CmpStringWithTemplate(const char* const str, const char* const temp,
                      const size_t start, const size_t end) {
  for (size_t i = start; i <= end; ++i) {
    if (str[i]          != temp[i - start] || 
        str[i]          == '\0'            ||
        temp[i - start] == '\0') {
      return false;
    }
  }

  if (temp[end - start + 1] != '\0') {
    return false;
  }

  return true;
}

void shush::format::Itoa(const int num, char* buf) {
  char tmp[30];
  _itoa(num, tmp, 10);
  memcpy(buf, tmp, strlen(tmp));
}

std::string shush::format::FormatString(const std::string& str,
    const std::map<std::string, std::string>& var_map) noexcept {
  std::string out_str;
  out_str.reserve(str.length());

  size_t start_i = 0;
  size_t start_j = 0;

  for (size_t i = 0, length = str.length(); i < length; ++i) {
    out_str += str[i];

    if (str[i] == '{') {
      start_i = i;
      start_j = out_str.length() - 1;
    } else if (str[i] == '}') {
      if (CmpStringWithTemplate(&str[0], "{date}", start_i, i)) {
        char date[DATE_LENGTH + 1];
        GetCurrentDate(date);
        out_str.replace(start_j, out_str.length() - start_j, date);

        start_i = i;
        start_j = out_str.length() - 1;
      } else {
        std::string var = str.substr(start_i + 1, i - start_i - 1);

        if (var_map.find(var) != var_map.end()) {
          out_str.replace(start_j, out_str.length() - start_j, var_map.at(var));
        }
      }
    }
  }

  return out_str;
}

void shush::format::FormatString(const char* const str, char* out_str) noexcept {
  size_t start_i = 0;
  size_t start_j = 0;

  for (size_t i = 0, j = 0; str[i] != '\0'; ++i) {
    out_str[j++] = str[i];

    if (str[i] == '{') {
      start_i = i;
      start_j = j - 1;
    } else if (str[i] == '}') {
      if (CmpStringWithTemplate(str, "{date}", start_i, i)) {
        char date[DATE_LENGTH + 1];
        GetCurrentDate(date);
        memcpy(&out_str[start_j], date, DATE_LENGTH);
        j = start_j + DATE_LENGTH;

        start_i = i;
        start_j = j - 1;
      }
    }

    if (str[i + 1] == '\0') {
      out_str[j] = '\0';
    }
  }
}

void shush::format::FormatDecimalInPos(char* const str,
    const size_t pos, const int val) noexcept {
  if (val < 10) {
    Itoa(val, &str[pos + 1]);
  } else {
    Itoa(val, &str[pos]);
  }
}

void shush::format::GetCurrentDate(char* out_str) noexcept {
  time_t now;
  time(&now);
  tm local_time[1];
  localtime_s(local_time, &now);

  memcpy(out_str, "0000.00.00-00:00:00", DATE_LENGTH);
  out_str[DATE_LENGTH] = '\0';

  Itoa(local_time->tm_year + 1900, out_str);

  FormatDecimalInPos(out_str, 5, local_time->tm_mon + 1);
  FormatDecimalInPos(out_str, 8, local_time->tm_mday);
  FormatDecimalInPos(out_str, 11, local_time->tm_hour);
  FormatDecimalInPos(out_str, 14, local_time->tm_min);
  FormatDecimalInPos(out_str, 17, local_time->tm_sec);
}
