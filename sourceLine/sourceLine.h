#pragma once

#include <string>
#include <string_view>
#include <deque>
#include <optional>

#include "Arithmetic/tryte.h"
#include "Architecture/sixTritCPU.h"



class SourceLine
{
public:

    SourceLine(const std::string_view &line) : tokenised{splitLine(std::string{removeComment(line)})}
    {}

    auto &tokens() const noexcept
    {
        return tokenised;
    }

    auto &tokens() noexcept
    {
        return tokenised;
    }


    std::optional<std::string_view> asString(int index) const noexcept
    {
        if(index >= tokenised.size())
        {
            return std::nullopt;
        }
        else
        {
            return tokenised.at(index);
        }
    }

    std::optional<tryte>                                asTryte     (int index) const noexcept;
    std::optional<trybble>                              asTrybble   (int index) const noexcept;
    std::optional<Architecture::sixTrit::OpCode>        asOpCode    (int index) const noexcept;
    std::optional<Architecture::sixTrit::Register>      asRegister  (int index) const noexcept;
    std::optional<Architecture::Condition>              asCondition (int index) const noexcept;
    std::optional<Architecture::CpuControl>             asCpuControl(int index) const noexcept;
    

private:

    std::optional<int>            asDecimal(int index) const noexcept;

private:

    [[nodiscard]] static std::string_view              removeComment(const std::string_view  &line);
    [[nodiscard]] static std::deque<std::string>       splitLine    (const std::string &text);

    std::deque<std::string>  tokenised;
};


#pragma comment(lib,"sourceline")