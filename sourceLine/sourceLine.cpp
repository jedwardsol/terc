#include "sourceLine.h"
#include <charconv>
#include <map>

[[nodiscard]] std::string_view  SourceLine::removeComment(const std::string_view  &line)
{
    std::string_view    text{line};

    auto comment = text.find(';');

    if(comment != text.npos)
    {
        text.remove_suffix( text.size() - comment);
    }

    return text;
}


[[nodiscard]] std::string_view  SourceLine::trimDelimiters(const std::string_view  &text)
{

    auto result{text};

    auto start = result.find_first_not_of(delimiters);

    if(start != result.npos)
    {
        result.remove_prefix( start);
    }
    else
    {
        // it's all delimiters;
        result = std::string_view{};
    }
    auto end = result.find_last_not_of(delimiters);

    if(end != result.npos)
    {
        result.remove_suffix( result.size() - (end+1));
    }

    return result;
}


[[nodiscard]] std::vector<std::string_view>   SourceLine::splitLine(std::string_view  text)
{
    std::vector<std::string_view>   tokens;

    text = trimDelimiters(text);

    while(!text.empty())
    {
        auto delimiter = text.find_first_of(delimiters);
        auto token     = text.substr(0,delimiter);

        if(delimiter == text.npos)
        {
            text = std::string_view{};
        }
        else
        {
            text.remove_prefix(delimiter);
            text = trimDelimiters(text);
        }

        tokens.push_back(trimDelimiters(token));                

    }

    return tokens;
}


std::optional<tryte> SourceLine::asTryte(int index) const noexcept
{
    {
        auto string = asString(index);

        if(!string)
        {
            return std::nullopt;
        }

        try
        {
            tryte   t{string.value()};
                
            return t;
        }
        catch(...)
        {
        }
    }
    {
        auto number = asDecimal(index);

        if(!number)
        {
            return std::nullopt;
        }

        try
        {
            tryte   t{number.value()};
                
            return t;
        }
        catch(...)
        {
        }
    }

    return std::nullopt;
}



std::optional<int> SourceLine::asDecimal(int index) const noexcept
{
    auto string = asString(index);

    if(!string)
    {
        return std::nullopt;
    }

    try
    {
        int  value{} ;
        
        auto start = string.value().data();
        auto end   = start + string.value().size();
        
        const auto result = std::from_chars(start, end, value, 10);

        if(    result.ec  == std::errc{}
           &&  result.ptr == end)
        {
            return value;
        }
    }
    catch(...)
    {
    }

    return std::nullopt;
}



std::optional<Architecture::sixTrit::Register> SourceLine::asRegister(int index) const noexcept
{
    auto string = asString(index);

    if(!string)
    {
        return std::nullopt;
    }

    const static std::map<std::string_view,Architecture::sixTrit::Register> registers
    {
        {"RPC",   Architecture::sixTrit::Register::RPC},
        {"RSP",   Architecture::sixTrit::Register::RSP},   
        {"RFlags",Architecture::sixTrit::Register::RFlags},
        {"RRA",   Architecture::sixTrit::Register::RRA},   
        {"REXC",  Architecture::sixTrit::Register::REXC},  
        {"REXA",  Architecture::sixTrit::Register::REXA},  
        {"Rn7",   Architecture::sixTrit::Register::Rn7},
        {"Rn6",   Architecture::sixTrit::Register::Rn6},
        {"Rn5",   Architecture::sixTrit::Register::Rn5},
        {"Rn4",   Architecture::sixTrit::Register::Rn4},
        {"Rn3",   Architecture::sixTrit::Register::Rn3},
        {"Rn2",   Architecture::sixTrit::Register::Rn2},
        {"Rn1",   Architecture::sixTrit::Register::Rn1},
        {"R0",    Architecture::sixTrit::Register::R0},
        {"R1",    Architecture::sixTrit::Register::R1},
        {"R2",    Architecture::sixTrit::Register::R2},
        {"R3",    Architecture::sixTrit::Register::R3},
        {"R4",    Architecture::sixTrit::Register::R4},
        {"R5",    Architecture::sixTrit::Register::R5},
        {"R6",    Architecture::sixTrit::Register::R6},
        {"R7",    Architecture::sixTrit::Register::R7},
        {"R8",    Architecture::sixTrit::Register::R8},
        {"R9",    Architecture::sixTrit::Register::R9},
        {"R10",   Architecture::sixTrit::Register::R10},
        {"R11",   Architecture::sixTrit::Register::R11},
        {"R12",   Architecture::sixTrit::Register::R12},
        {"R13",   Architecture::sixTrit::Register::R13},
    };

    auto found = registers.find(string.value());

    if(found != registers.end())
    {
        return found->second;
    }
    
    return std::nullopt;           
}


