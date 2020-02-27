/*******************************************************************************
 * CLI - A simple command line interface.
 * Copyright (C) 2019 Daniele Pallastrelli
 *
 * Boost Software License - Version 1.0 - August 17th, 2003
 *
 * Permission is hereby granted, free of charge, to any person or organization
 * obtaining a copy of the software and accompanying documentation covered by
 * this license (the "Software") to use, reproduce, display, distribute,
 * execute, and transmit the Software, and to prepare derivative works of the
 * Software, and to permit third-parties to whom the Software is furnished to
 * do so, all subject to the following:
 *
 * The copyright notices in the Software and this entire statement, including
 * the above license grant, this restriction and the following disclaimer,
 * must be included in all copies of the Software, in whole or in part, and
 * all derivative works of the Software, unless such copies or derivative
 * works are solely in the form of machine-executable object code generated by
 * a source language processor.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
 * SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
 * FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 ******************************************************************************/

#ifndef CLI_DETAIL_SPLIT_H_
#define CLI_DETAIL_SPLIT_H_

#include <string>
#include <vector>
#include <algorithm>

namespace cli
{
namespace detail
{


inline void split(std::vector<std::string>& strs, const std::string& input)
{
    enum class State { space, word, sentence };
    char sentenceStartChar;
    State state = State::space;

    strs.clear();

    for (char c: input)
    {
        switch(state)
        {
            case State::space:
                if (c == ' ' || c == '\t' || c == '\n')
                {
                    // do nothing
                }
                else if (c == '"' || c == '\'')
                {
                    sentenceStartChar = c;
                    state = State::sentence;
                    strs.push_back("");
                }
                else
                {
                    state = State::word;
                    strs.push_back(std::string(1, c));
                }
                break;
            case State::word:
                if (c == ' ' || c == '\t' || c == '\n')
                {
                    state = State::space;
                }
                else if (c == '"' || c == '\'')
                {
                    sentenceStartChar = c;
                    state = State::sentence;
                    strs.push_back("");
                }
                else
                {
                    assert(!strs.empty());
                    strs.back() += c;
                }
                break;
            case State::sentence:
                if (c == sentenceStartChar)
                {
                    state = State::space;
                }
                else
                {
                    assert(!strs.empty());
                    strs.back() += c;
                }
        }
    }

    // remove null entries from the vector:
    strs.erase(
        std::remove_if(
            strs.begin(),
            strs.end(),
            [](const std::string& s){ return s.empty(); }
        ),
        strs.end()
    );
}

} // namespace detail
} // namespace cli

#endif // CLI_DETAIL_SPLIT_H_
