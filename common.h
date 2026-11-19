#pragma once

#include <cstdint>
#include <string>
#include <vector>

// Общие функции для ввода и команд.
std::string upper(std::string text);
std::vector<std::string> words(const std::string& text);
bool parseInt64(const std::string& text, int64_t& number);
std::string readLine();
int64_t readNum();
