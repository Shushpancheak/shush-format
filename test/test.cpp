#include <gtest/gtest.h>
#include <string>
#include <shush-format.hpp>

using namespace shush::format;


// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// - - - - - - - - - TESTS WITHOUT VARS- - - - - - - - - - - - - - - - - - - -
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

inline std::string Format(const std::string& str) {
  return FormatString(str, std::map<std::string, std::string>());
}

inline void AssertDynamicFormatNoVars(const std::string& str) {
  ASSERT_EQ(Format(str), str);
}

inline void FormatStatic(const char* const str, char* out_str) {
  FormatString(str, out_str);
}

inline void AssertStaticFormatNoVars(const char* const str, char* out_str) {
  FormatStatic(str, out_str);
  ASSERT_EQ(std::string(str), std::string(out_str));
}

TEST(NO_VARS, DYNAMIC) {
  AssertDynamicFormatNoVars("ABC");
  AssertDynamicFormatNoVars("1234567890-=qwertyuiop[]\asdfghjkl;'zxcvbnm,./");
  AssertDynamicFormatNoVars("");

  std::string all_chars;
  for (char ch = 1; ch != '\0'; ++ch) {
    if (ch != '{' && ch != '}') {
      all_chars += ch;
    }
  }
  AssertDynamicFormatNoVars(all_chars);
}

TEST(NO_VARS, STATIC) {
  std::string str = "ABC";
  char static_str_out[1000];

  AssertStaticFormatNoVars(&str[0], static_str_out);

  str = "1234567890-=qwertyuiop[]\asdfghjkl;'zxcvbnm,./";
  AssertStaticFormatNoVars(&str[0], static_str_out);

  str = "";
  for (char ch = 1; ch != '\0'; ++ch) {
    if (ch != '{' && ch != '}') {
      str += ch;
    }
  }
  AssertStaticFormatNoVars(&str[0], static_str_out);
}

std::vector<std::string> bracket_strings_to_test = {
    "ABC{}",
    "{}ABC",
    "{ABC}",
    "{A}BC",
    "A{B}C",
    "AB{C}",
    "{AB}C",
    "A{BC}",
    "{}",
    "}{",
    "}}{",
    "{}}",
    "}{}{}}",
    "{{{{}}}}}}}}",
    "{}}}}{}{}}",
    "{{{{{}{{}{}{}{}}{}{}}",
    "{{}}}}{}}}{{}{{",
    "}}{}{{}}{}{}{}}{}",
    "{}{}{}{{{{{}{}{}{}}",
    "{}}{}{}{}{}{}{}}",
    "KEK{KEK{KEK{KEK}}}}}}",
    "}}}}}}}}}}}{{{{{{{{}}}}}}}}}}}"
  };

TEST(NO_VARS, WITH_BRACKETS_DYNAMIC) {
  for (auto& str : bracket_strings_to_test) {
    AssertDynamicFormatNoVars(str);
  }
}

TEST(NO_VARS, WITH_BRACKETS_STATIC) {
  char static_str_out[1000];

  for (auto& str : bracket_strings_to_test) {
    AssertStaticFormatNoVars(&str[0], static_str_out);
  }
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// - - - - - - - - - - - - - -TESTS WITH VARS- - - - - - - - - - - - - - - - -
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

size_t CountDigits(const std::string& str) {
  size_t cnt = 0;
  for (auto& ch : str) {
    if (isdigit(ch)) {
      ++cnt;
    }
  }

  return cnt;
}

// The number states how many correct {date}'s there will be.
std::vector<std::string> test_1_date = {
  "asdf{date}asdf",
  "{date}{date[date}",
  "{{{{date]}}{{}{date}das",
  "}{}}}{}}}}{{{date}{}}date{"
};

std::vector<std::string> test_2_date = {
  "{date}}{{date}{{{{{{",
  "l{pdate}flbdp{date}flb{{DA}Sf][}{D}V{Dv[D}{VD{V{}DV{}A}}}AS{}{AS}[}ASd}ASD{}ASD{AS}{DAE{date}{Date} {date",
  "{{{{date}}}{{}{date}das",
  "}{}}}{}}}}{{{date}{}}}{date}{"
};

TEST(VARS, DATE_DYNAMIC) {
  for (auto& str : test_1_date) {
    // - 5 because there are 5 non digit symbols in date string.
    ASSERT_EQ(CountDigits(Format(str)), DATE_LENGTH - 5);
  }
  for (auto& str : test_2_date) {
    ASSERT_EQ(CountDigits(Format(str)), 2 * (DATE_LENGTH - 5));
  }
}

TEST(VARS, DATE_STATIC) {
  char out[1000];

  for (auto& str : test_1_date) {
    // - 5 because there are 5 non digit symbols in date string.
    FormatStatic(&str[0], out);
    ASSERT_EQ(CountDigits(out), DATE_LENGTH - 5);
  }
  for (auto& str : test_2_date) {
    FormatStatic(&str[0], out);
    ASSERT_EQ(CountDigits(out), 2 * (DATE_LENGTH - 5));
  }
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// - - - - - - - - - - - - - -TESTS WITH CUSTOM VARS - - - - - - - - - - - - - 
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

std::string
FormatWithCustomVars(const std::string& str,
                     std::map<std::string, std::string> var_map) {
  return FormatString(str, var_map);
}

std::vector<
  std::tuple<
    std::string,
    std::map<std::string, std::string>,
    std::string
  >
> test_custom_vars = {
  {"{kek}", {{"kek", "LOL"}}, "LOL"},
  {"keklol{qwe}{{{]{{}{]]]{}][{{{qwe}{qwe}{{w}}", {{"qwe", "|"}, {"w", "{PRIKOL}"}}, "keklol|{{{]{{}{]]]{}][{{||{{PRIKOL}}"},
  {"{MN{O}{G}{O{H{ODOV{OCHKA}}}}", {{"O", "a"}, {"OCHKA", "in chess"}}, "{MNa{G}{O{H{ODOVin chess}}}"}
};

TEST(CUSTOM_VARS, DYNAMIC) {
  for (auto& item : test_custom_vars) {
    ASSERT_EQ(FormatWithCustomVars(std::get<0>(item), std::get<1>(item)),
              std::get<2>(item));
  }
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}