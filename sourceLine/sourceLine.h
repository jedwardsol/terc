#include <string>
#include <string_view>
#include <vector>

class SourceLine
{
public:

    SourceLine(const std::string_view &line) : tokenised{splitLine(removeComment(line))}
    {}

    const auto &tokens() const
    {
        return tokenised;
    }


private:

    const static inline std::string_view    delimiters{", \t"};

    [[nodiscard]] static std::string_view              removeComment(const std::string_view  &line);
    [[nodiscard]] static std::string_view              trimDelimiters(const std::string_view  &text);
    [[nodiscard]] static std::vector<std::string_view> splitLine(std::string_view  text);


    std::vector<std::string_view>  tokenised;
};


#pragma comment(lib,"sourceline")