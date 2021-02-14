#include "assembler.h"

void Assembler::parseCode     (const SourceLine &source)
{
    const auto   &first = source.asString(0).value();

// Label

    if(first[0] == ':')
    {
        addCodeSymbol(first);

        if(source.tokens().size() > 1)
        {
            SourceLine rest{source};

            rest.tokens().pop_front();

            parseCode(rest);
        }
    }

// Code

    else
    {
        auto opCode = source.asOpCode(0);

        if(!opCode)
        {
            error("Unrecognised instruction()");
        }


        switch(opCode.value())
        {
        case Architecture::sixTrit::OpCode::CpuControl:             // condition        CpuControl                                                     
            parseCode_OpCodeConditionCpuControl(source);
            break;

        case Architecture::sixTrit::OpCode::CallI:                  // condition        immediate destination                                          
        case Architecture::sixTrit::OpCode::CallR:                  // condition        low: register destination                                      
        case Architecture::sixTrit::OpCode::JmpI:                   // condition        immediate destination                                          
        case Architecture::sixTrit::OpCode::JmpR:                   // condition        low: register destination                                      
        case Architecture::sixTrit::OpCode::LoadImmediate:          // destination      immediate                           dest = immediate           
        case Architecture::sixTrit::OpCode::Copy:                   // destination      low:source                          destination = source       
        case Architecture::sixTrit::OpCode::Out:                    // source           low:port                            write source to port       
        case Architecture::sixTrit::OpCode::In:                     // destination      low:port                            read port to destination   
        case Architecture::sixTrit::OpCode::Load:                   // destination      low:regsource      high:offset      dest = [source+offset]     
        case Architecture::sixTrit::OpCode::Store:                  // source           low:regdest        high:offset      [dest+offset] = source     
        case Architecture::sixTrit::OpCode::Push:                   // source           unused                              SP-- stack[SP]=src         
        case Architecture::sixTrit::OpCode::Pop:                    // destination      unused                              dest=stack[SP] SP++        
        case Architecture::sixTrit::OpCode::CmpI:                   // Reg X            immediate                                                      
        case Architecture::sixTrit::OpCode::CmpR:                   // Reg X            low:: Reg Y                                                    
            break;
        }
    }
}



void Assembler::parseCode_OpCodeConditionCpuControl    (const SourceLine &source)
{
    auto opCode     = source.asOpCode    (0);
    auto condition  = source.asCondition (1);
    auto cpuControl = source.asCpuControl(2);

    if(!condition)
    {
        error("Invalid condition");
    }

    if(!cpuControl)
    {
        error("Invalid cpuControl");
    }

    tryte  first
    {
        trybble{ static_cast<int>(opCode.value())    },
        trybble{ static_cast<int>(condition.value()) }
    };

    tryte  second
    {
        trybble{ static_cast<int>(cpuControl.value())},
        trybble{ 0 }
    };

    addInstruction(first,second);
}


void Assembler::addInstruction                         (tryte first, tryte second)
{
    code[currentCodePosition]   = first;
    code[currentCodePosition+1] = second;
    incrementCodePosition();
}
