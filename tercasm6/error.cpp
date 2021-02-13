#include <iomanip>
#include <stdexcept>
#include <string>
#include <sstream>
#include "assembler.h"

std::string Assembler::formatMessage(const std::string &type, const std::string &message) const
{
    std::ostringstream beginning;
    std::ostringstream all;

    beginning << type << " : "s << message ;

    all << std::left << std::setw(60) << beginning.str() 
        << " At line "s << std::setw(3) << std::to_string(currentLineNumber) 
        << " | " << currentLine;

    return all.str();
}

void Assembler::error         (const std::string &error) const
{
    throw std::runtime_error(  formatMessage("Error",error));
}

void Assembler::warning        (const std::string &warning) const
{
    std::cout << formatMessage("Warning", warning) << "\n";
}
