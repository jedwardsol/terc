#include <string>
#include <string_view>
#include <vector>
#include <optional>

#include "Arithmetic/tryte.h"
#include "Architecture/sixTritCPU.h"



class SourceLine
{
public:

    SourceLine(const std::string_view &line) : tokenised{splitLine(removeComment(line))}
    {}

    const auto &tokens() const noexcept
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
    std::optional<Architecture::sixTrit::Register>      asRegister  (int index) const noexcept;
    std::optional<Architecture::Condition>              asCondition (int index) const noexcept;
    std::optional<Architecture::CpuControl>             asCpuControl(int index) const noexcept;
    

private:

    std::optional<int>            asDecimal(int index) const noexcept;

private:

    const static inline std::string_view    delimiters{", \t"};

    [[nodiscard]] static std::string_view              removeComment(const std::string_view  &line);
    [[nodiscard]] static std::string_view              trimDelimiters(const std::string_view  &text);
    [[nodiscard]] static std::vector<std::string_view> splitLine(std::string_view  text);


    std::vector<std::string_view>  tokenised;
};


#pragma comment(lib,"sourceline")