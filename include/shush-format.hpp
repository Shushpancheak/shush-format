#pragma once
#include <string>
#include <map>
#include <vector>
#include <ctime>
#include <cstring>

namespace shush {
/**
 * Easy-to-use nothrow text formatter.
 */
namespace format {

/**
 * @brief The length of string that is returned by GetCurrentDate().
 * Remember to declare arrays of size DATE_LENGTH + 1 to have
 * enough space for storing date.
 */
static const size_t DATE_LENGTH = 19;

/**
 * @brief Compare a part of string with a part of template.
 * Comparison starts from "start" in str and 0 in temp and ends when
 * one of the strings ends.
 *
 * @param[in] str   a string to compare with template
 * @param[in] temp  a template
 * @param[in] start start position in str to begin comparison
 * @param[in] end   where to stop in str.
 *
 */
static bool CmpStringWithTemplate(const char* const str, const char* const temp,
                                  size_t start, const size_t end);

/**
 * Uses standart _itoa, but doesn't overwrite the string completely, just
 * overwriting needed symbols.
 */
static void Itoa(int num, char* buf);

/**
 * @brief Format your string.
 *
 * @param[in] str     unformatted string to format
 * @param[in] var_map map containing key-values of names of variables
 *                    in str and what to write instead.
 * @returns The formatted string.
 *
 * @details Available variables:
 * - {YOUR_NAME} - write your formatted text. Be sure to
 *   provide a map var_map containing key = "YOUR_NAME", value =
 *   YOUR_VALUE. If it's not found, the value will be "{YOUR_NAME}"
 * - {date} - write current date and time in format yyyy.mm.dd-hh.mm.ss
 */
extern std::string FormatString(
    const std::string& str,
    const std::map<std::string, std::string>& var_map) noexcept;

/**
 * @brief Static implementation of FormatString. Does not support variables
 * with custom names.
 *
 * @param[in]  str     unformatted string to format
 * @param[out] out_str output
 *
 * @details Available variables:
 * - {date} - write current date and time in format yyyy.mm.dd-hh:mm:ss
 */
extern void FormatString(const char* const str, char* out_str) noexcept;

/**
 * @brief Used for placing decimals < 99 in corresponding position.
 *
 * @param[in] str String to change. It should look like this: *00*, where
 *                zeros will be the place where a number should be put.
 * @param[in] pos Position of "00" you want to change.
 * @param[in] val Value to convert ot string.
 */
extern void FormatDecimalInPos(char* const str,
    size_t pos, int val) noexcept;

/**
 * @param[out] out_str Current date and time in format yyyy.mm.dd-hh:mm:ss (DATE_LENGTH bytes)
 */
extern void GetCurrentDate(char* out_str) noexcept;
};
}
