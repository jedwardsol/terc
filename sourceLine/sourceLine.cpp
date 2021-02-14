#include "sourceLine.h"
#include <charconv>
#include <map>
#include <boost/tokenizer.hpp>
#include <boost/range/iterator_range.hpp>

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

   
std::deque<std::string>   SourceLine::splitLine(const std::string &text)
{
 
    boost::escaped_list_separator<char> seperators{"\\"s, " \t,"s,  "\""s};
  
    boost::tokenizer<boost::escaped_list_separator<char>> tokens(text, seperators);

    std::deque<std::string> result;

    for(const auto &token : tokens)
    {
        if(!token.empty())
        {
            result.push_back(token);
        }
    }

    return result;
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


std::optional<tryte> SourceLine::asTrybble(int index) const noexcept
{
    {
        auto string = asString(index);

        if(!string)
        {
            return std::nullopt;
        }

        try
        {
            std::string  expanded = "000"s + std::string(string.value());

            tryte   t{expanded};
                
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
            if(    number.value() >  maxValue(3)
               ||  number.value() < -maxValue(3))
            {
                return std::nullopt;
            }

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



std::optional<Architecture::sixTrit::OpCode> SourceLine::asOpCode(int index) const noexcept
{
    auto string = asString(index);

    if(!string)
    {
        return std::nullopt;
    }

    const static std::map<std::string_view,Architecture::sixTrit::OpCode> opCodes
    {
        {"CallI",       Architecture::sixTrit::OpCode::CallI},        
        {"CallR",       Architecture::sixTrit::OpCode::CallR},        
        {"JmpI",        Architecture::sixTrit::OpCode::JmpI},         
        {"JmpR",        Architecture::sixTrit::OpCode::JmpR},         
        {"CpuControl",  Architecture::sixTrit::OpCode::CpuControl},
        {"LoadI",       Architecture::sixTrit::OpCode::LoadImmediate},
        {"Copy",        Architecture::sixTrit::OpCode::Copy},         
        {"Out",         Architecture::sixTrit::OpCode::Out},          
        {"In",          Architecture::sixTrit::OpCode::In},           
        {"Load",        Architecture::sixTrit::OpCode::Load},         
        {"Store",       Architecture::sixTrit::OpCode::Store},        
        {"Push",        Architecture::sixTrit::OpCode::Push},         
        {"Pop",         Architecture::sixTrit::OpCode::Pop},          
        {"CmpI",        Architecture::sixTrit::OpCode::CmpI},         
        {"CmpR",        Architecture::sixTrit::OpCode::CmpR},         
    };


    auto opCode = opCodes.find(string.value());

    if(opCode != opCodes.end())
    {
        return opCode->second;
    }
    
    return std::nullopt;           


    return std::optional<Architecture::sixTrit::OpCode>();
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

    auto reg = registers.find(string.value());

    if(reg != registers.end())
    {
        return reg->second;
    }
    
    return std::nullopt;           
}


std::optional<Architecture::Condition> SourceLine::asCondition(int index) const noexcept
{
    auto string = asString(index);

    if(!string)
    {
        return std::nullopt;
    }

    const static std::map<std::string_view,Architecture::Condition> conditions
    {
        {"False",   Architecture::Condition::AlwaysFalse},
        {"True",    Architecture::Condition::AlwaysTrue},

        {"CE",      Architecture::Condition::ConditionalExecuted},   
        {"NCE",     Architecture::Condition::ConditionalNotExecuted },

        {"P",       Architecture::Condition::Positive},      
        {"Z",       Architecture::Condition::Zero},          
        {"N",       Architecture::Condition::Negative},      
        {"NP",      Architecture::Condition::NotPositive},  
        {"NZ",      Architecture::Condition::NotZero},      
        {"NN",      Architecture::Condition::NotNegative},  

        {"GT",      Architecture::Condition::GreaterThan},        
        {"E",       Architecture::Condition::Equal},              
        {"LT",      Architecture::Condition::LessThan},           
        {"LE",      Architecture::Condition::LessThanOrEqual},    
        {"NE",      Architecture::Condition::NotEqual},           
        {"GE",      Architecture::Condition::GreaterOrEqual},     

        {">",       Architecture::Condition::GreaterThan},        
        {"==",      Architecture::Condition::Equal},              
        {"<",       Architecture::Condition::LessThan},           
        {"<=",      Architecture::Condition::LessThanOrEqual},    
        {"!=",      Architecture::Condition::NotEqual},           
        {">=",      Architecture::Condition::GreaterOrEqual},     
    };

    auto condition = conditions.find(string.value());

    if(condition != conditions.end())
    {
        return condition->second;
    }
    
    return std::nullopt;           
}


std::optional<Architecture::CpuControl> SourceLine::asCpuControl(int index) const noexcept
{
    auto string = asString(index);

    if(!string)
    {
        return std::nullopt;
    }

    const static std::map<std::string_view,Architecture::CpuControl> cpuControls
    {
        {"Nop",         Architecture::CpuControl::Nop},        
        {"Halt",        Architecture::CpuControl::Halt},              
        {"Breakpoint",  Architecture::CpuControl::Breakpoint},           
        {"Trace",       Architecture::CpuControl::Trace},    
        {"Invalid",     Architecture::CpuControl::Invalid},     
    };

    auto cpuControl = cpuControls.find(string.value());

    if(cpuControl != cpuControls.end())
    {
        return cpuControl->second;
    }
    
    return std::nullopt;           
}
