#include "sourceLine.h"
#include <charconv>

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
        
        
        const auto result = std::from_chars(start, end, value);

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
