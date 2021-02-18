#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
using namespace std::literals;
#include <map>


#include "Arithmetic/Arithmetic.h"
#include "Arithmetic/trint.h"
#include "Arithmetic/trint_std.h"

#include "terc6cpu/sixTritCPU.h"

using namespace Architecture;

std::ostream &operator<<(std::ostream &out,     Architecture::sixTrit::OpCode opCode)
{
    const static std::map<Architecture::sixTrit::OpCode,std::string_view> opCodes
    {
        {  Architecture::sixTrit::OpCode::CallI,         "CallI"},     
        {  Architecture::sixTrit::OpCode::CallR,         "CallR"},     
        {  Architecture::sixTrit::OpCode::JmpI,          "JmpI"},      
        {  Architecture::sixTrit::OpCode::JmpR,          "JmpR"},      
        {  Architecture::sixTrit::OpCode::CpuControl,    "CpuControl"},
        {  Architecture::sixTrit::OpCode::Assign,        "Assign"},    
        {  Architecture::sixTrit::OpCode::Copy,          "Copy"},      
        {  Architecture::sixTrit::OpCode::Out,           "Out"},       
        {  Architecture::sixTrit::OpCode::In,            "In"},        
        {  Architecture::sixTrit::OpCode::Load,          "Load"},      
        {  Architecture::sixTrit::OpCode::Store,         "Store"},     
        {  Architecture::sixTrit::OpCode::Push,          "Push"},      
        {  Architecture::sixTrit::OpCode::Pop,           "Pop"},       
        {  Architecture::sixTrit::OpCode::CmpI,          "CmpI"},      
        {  Architecture::sixTrit::OpCode::CmpR,          "CmpR"},      
        {  Architecture::sixTrit::OpCode::Neg,           "Neg"},       
        {  Architecture::sixTrit::OpCode::Shift,         "Shift"},     
        {  Architecture::sixTrit::OpCode::AddI,          "AddI"},      
        {  Architecture::sixTrit::OpCode::AddR,          "AddR"},      
    };

    auto found = opCodes.find(opCode);

    return out << found->second;
}

std::ostream &operator<<(std::ostream &out,     Architecture::sixTrit::Register reg)
{
    const static std::map<Architecture::sixTrit::Register,std::string_view> registers
    {
        {Architecture::sixTrit::Register::RPC,       "RPC"},   
        {Architecture::sixTrit::Register::RSP,       "RSP"},   
        {Architecture::sixTrit::Register::RFlags,    "RFlags"},
        {Architecture::sixTrit::Register::RRA,       "RRA"},   
        {Architecture::sixTrit::Register::REXC,      "REXC"},  
        {Architecture::sixTrit::Register::REXA,      "REXA"},  
        {Architecture::sixTrit::Register::Rn7,       "Rn7"},   
        {Architecture::sixTrit::Register::Rn6,       "Rn6"},   
        {Architecture::sixTrit::Register::Rn5,       "Rn5"},   
        {Architecture::sixTrit::Register::Rn4,       "Rn4"},   
        {Architecture::sixTrit::Register::Rn3,       "Rn3"},   
        {Architecture::sixTrit::Register::Rn2,       "Rn2"},   
        {Architecture::sixTrit::Register::Rn1,       "Rn1"},   
        {Architecture::sixTrit::Register::R0,        "R0"},    
        {Architecture::sixTrit::Register::R1,        "R1"},    
        {Architecture::sixTrit::Register::R2,        "R2"},    
        {Architecture::sixTrit::Register::R3,        "R3"},    
        {Architecture::sixTrit::Register::R4,        "R4"},    
        {Architecture::sixTrit::Register::R5,        "R5"},    
        {Architecture::sixTrit::Register::R6,        "R6"},    
        {Architecture::sixTrit::Register::R7,        "R7"},    
        {Architecture::sixTrit::Register::R8,        "R8"},    
        {Architecture::sixTrit::Register::R9,        "R9"},    
        {Architecture::sixTrit::Register::R10,       "R10"},   
        {Architecture::sixTrit::Register::R11,       "R11"},   
        {Architecture::sixTrit::Register::R12,       "R12"},   
        {Architecture::sixTrit::Register::R13,       "R13"},   
    };

    auto found = registers.find(reg);

    return out << found->second;
}


std::ostream &operator<<(std::ostream &out,     Architecture::Condition condition)
{
    const static std::map<Architecture::Condition,std::string_view> conditions
    {
        {Architecture::Condition::AlwaysFalse,              "False"},
        {Architecture::Condition::AlwaysTrue,               "True"}, 

        {Architecture::Condition::ConditionalExecuted,      "CE"},   
        {Architecture::Condition::ConditionalNotExecuted,   "NCE"},  

        {Architecture::Condition::Positive,                 "P"},    
        {Architecture::Condition::Zero,                     "Z"},    
        {Architecture::Condition::Negative,                 "N"},    
        {Architecture::Condition::NotPositive,              "NP"},   
        {Architecture::Condition::NotZero,                  "NZ"},   
        {Architecture::Condition::NotNegative,              "NN"},   

//      {Architecture::Condition::GreaterThan,              "GT"},   
//      {Architecture::Condition::Equal,                    "E"},    
//      {Architecture::Condition::LessThan,                 "LT"},   
//      {Architecture::Condition::LessThanOrEqual,          "LE"},   
//      {Architecture::Condition::NotEqual,                 "NE"},   
//      {Architecture::Condition::GreaterOrEqual,           "GE"},   

        {Architecture::Condition::GreaterThan,              ">"},    
        {Architecture::Condition::Equal,                    "=="},   
        {Architecture::Condition::LessThan,                 "<"},    
        {Architecture::Condition::LessThanOrEqual,          "<="},   
        {Architecture::Condition::NotEqual,                 "!="},   
        {Architecture::Condition::GreaterOrEqual,           ">="},   
        
        {Architecture::Condition::Overflow,                 "O"},    
        {Architecture::Condition::OverflowPositive,         "OP"},        
        {Architecture::Condition::OverflowNegative,         "ON"},     
        {Architecture::Condition::NotOverflow,              "NO"},   
    };

    auto found = conditions.find(condition);

    return out << found->second;
}


std::ostream &operator<<(std::ostream &out,     Architecture::CpuControl cpuControl)
{
    const static std::map<Architecture::CpuControl,std::string_view> cpuControls
    {
        {Architecture::CpuControl::Halt,          "Halt"},            
        {Architecture::CpuControl::Breakpoint,    "Breakpoint"},         
        {Architecture::CpuControl::Trace,         "Trace"},       
        {Architecture::CpuControl::Invalid,       "Invalid"},     
    };

    auto found = cpuControls.find(cpuControl);

    return out << found->second;
}






std::string  disassembleRegisterInstructions(tryte  operation, tryte operand)
{
    std::ostringstream  str;
    auto                opcode = static_cast<Architecture::sixTrit::OpCode>  (static_cast<int>(operation.halves().first));
    auto                opreg  = static_cast<Architecture::sixTrit::Register>(static_cast<int>(operation.halves().second));



    str<< std::left << std::setw(4) << opreg << " ";

    switch(opcode)
    {
    case Architecture::sixTrit::OpCode::Assign:
    case Architecture::sixTrit::OpCode::AddI:
    case Architecture::sixTrit::OpCode::CmpI:
        str << operand;
        break;

    case Architecture::sixTrit::OpCode::Copy:
    case Architecture::sixTrit::OpCode::CmpR:
    {
        auto                srcreg  = static_cast<Architecture::sixTrit::Register>(static_cast<int>(operand.halves().first));
        str << srcreg;
        break;
    }


    case Architecture::sixTrit::OpCode::AddR:
    case Architecture::sixTrit::OpCode::Load:
    case Architecture::sixTrit::OpCode::Store:
    {
        auto                srcreg  = static_cast<Architecture::sixTrit::Register>(static_cast<int>(operand.halves().first));
        str << std::setw(4) << srcreg << " " << operand.halves().second;
        break;
    }


    case Architecture::sixTrit::OpCode::Out:
    case Architecture::sixTrit::OpCode::In:
    case Architecture::sixTrit::OpCode::Shift:
        str << std::setw(4) << operand.halves().second;
        break;

    case Architecture::sixTrit::OpCode::Neg:
    case Architecture::sixTrit::OpCode::Push:
    case Architecture::sixTrit::OpCode::Pop:
        break;
    }

    return str.str();
}

std::string  disassembleConditionalInstructions(tryte  operation, tryte operand)
{
    std::ostringstream  str;
    auto                opcode    = static_cast<Architecture::sixTrit::OpCode>(static_cast<int>(operation.halves().first));
    auto                condition = static_cast<Architecture::Condition>      (static_cast<int>(operation.halves().second));

    str<< std::left << std::setw(4) << condition << " ";

    switch(opcode)
    {
    case Architecture::sixTrit::OpCode::CallI:
    case Architecture::sixTrit::OpCode::JmpI:
        str << operand;
        break;


    case Architecture::sixTrit::OpCode::CallR:
    case Architecture::sixTrit::OpCode::JmpR:
    {
        auto                srcreg  = static_cast<Architecture::sixTrit::Register>(static_cast<int>(operand.halves().first));
        str << srcreg ;
        break;
    }


    case Architecture::sixTrit::OpCode::CpuControl:
    {
        auto                cpuControl = static_cast<Architecture::CpuControl>(static_cast<int>(operand.halves().first));
        str << cpuControl;
        break;

    }
    }
    return str.str();

}
          


std::string  disassemble(tryte  operation, tryte operand)
{
    std::ostringstream  str;
    auto                opCode = static_cast<Architecture::sixTrit::OpCode>  (static_cast<int>(operation.halves().first));
    
    str<< std::left << std::setw(10) << opCode << " ";

    
    if(isConditionalInstruction(opCode))
    {
        str << disassembleConditionalInstructions(operation, operand);
    }
    else
    {
        str << disassembleRegisterInstructions(operation, operand);
    }

    return str.str();
}


