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
/////
// condition

        case Architecture::sixTrit::OpCode::CpuControl:             // condition        CpuControl                                                     
            parseCode_OpCodeConditionCpuControl(source);
            break;

        case Architecture::sixTrit::OpCode::CallI:                  // condition        immediate destination                                          
        case Architecture::sixTrit::OpCode::JmpI:                   // condition        immediate destination                                          
            parseCode_OpCodeConditionImmediate(source);
            break;

        case Architecture::sixTrit::OpCode::CallR:                  // condition        low: register destination                                      
        case Architecture::sixTrit::OpCode::JmpR:                   // condition        low: register destination                                      
            parseCode_OpCodeConditionRegister(source);
            break;


/////
// register


        case Architecture::sixTrit::OpCode::Push:                   // source           unused                              SP-- stack[SP]=src         
            parseCode_OpCodeRegister(source,RegisterIs::Source);
            break;

        case Architecture::sixTrit::OpCode::Neg:                    // reg              unused                              reg = -reg
        case Architecture::sixTrit::OpCode::Pop:                    // destination      unused                              dest=stack[SP] SP++        
            parseCode_OpCodeRegister(source,RegisterIs::Destination);
            break;

        case Architecture::sixTrit::OpCode::Assign:                 // destination      immediate                           dest = immediate           
        case Architecture::sixTrit::OpCode::AddI:                   // Reg X            immediate                                                      
            parseCode_OpCodeRegisterImmediate(source,RegisterIs::Destination);
            break;

        case Architecture::sixTrit::OpCode::CmpI:                   // Reg X            immediate                                                      
            parseCode_OpCodeRegisterImmediate(source,RegisterIs::Source);
            break;

        case Architecture::sixTrit::OpCode::Copy:                   // destination      low:source                          destination = source       
            parseCode_OpCodeRegisterRegister(source,RegisterIs::Destination);
            break;

        case Architecture::sixTrit::OpCode::CmpR:                   // Reg X            low:: Reg Y                                                    
            parseCode_OpCodeRegisterRegister(source,RegisterIs::Source);
            break;

        case Architecture::sixTrit::OpCode::Out:                    // source           low:port                            write source to port       
            parseCode_OpCodeRegisterTrybble(source,RegisterIs::Source);
            break;

        case Architecture::sixTrit::OpCode::In:                     // destination      low:port                            read port to destination   
        case Architecture::sixTrit::OpCode::Shift:                  // reg              low:N                               reg<<=N or reg >>=N
            parseCode_OpCodeRegisterTrybble(source,RegisterIs::Destination);
            break;

        case Architecture::sixTrit::OpCode::Store:                  // source           low:regdest        high:offset      [dest+offset] = source     
            parseCode_OpCodeRegisterRegisterTrybble(source,RegisterIs::Source);
            break;

        case Architecture::sixTrit::OpCode::Load:                   // destination      low:regsource      high:offset      dest = [source+offset]     
        case Architecture::sixTrit::OpCode::AddR:                   // reg              low:reg2   high: direction          reg += reg2 * sign(dir)         E S O           
            parseCode_OpCodeRegisterRegisterTrybble(source,RegisterIs::Destination);
            break;

        default:
            error("Unhandled instruction");
        }
    }
}

void Assembler::addInstruction                         (tryte first, tryte second)
{
    code[currentCodePosition]   = first;
    code[currentCodePosition+1] = second;
    incrementCodePosition();
}

//////////////////


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


void Assembler::parseCode_OpCodeConditionImmediate     (const SourceLine &source)
{
    auto opCode     = source.asOpCode    (0);
    auto condition  = source.asCondition (1);
    
    if(!condition)
    {
        error("Invalid condition");
    }


    auto operand = parseImmediate (source,2);


    tryte  first
    {
        trybble{ static_cast<int>(opCode.value())    },
        trybble{ static_cast<int>(condition.value()) }
    };

    addInstruction(first,operand);
}


void Assembler::parseCode_OpCodeConditionRegister   (const SourceLine &source)
{
    auto opCode     = source.asOpCode    (0);
    auto condition  = source.asCondition (1);
    auto reg        = source.asRegister  (2);

    if(!condition)
    {
        error("Invalid condition");
    }

    if(!reg)
    {
        error("Invalid register");
    }

    tryte  first
    {
        trybble{ static_cast<int>(opCode.value())    },
        trybble{ static_cast<int>(condition.value()) }
    };

    tryte  second
    {
        trybble{ static_cast<int>(reg.value()) } ,
        trybble{ 0 }
    };

    addInstruction(first,second);
}



////////////////////////


void Assembler::checkReadOnlyRegister(Architecture::sixTrit::Register reg)
{
    if(   reg == Architecture::sixTrit::Register::REXA
       || reg == Architecture::sixTrit::Register::REXC
       || reg == Architecture::sixTrit::Register::RFlags)
    {
        error("Register is read-only");
    }
}


void Assembler::parseCode_OpCodeRegister               (const SourceLine &source, RegisterIs registerIs)
{
    auto opCode     = source.asOpCode    (0);
    auto reg        = source.asRegister  (1);

    if(!reg)
    {
        error("Invalid register");
    }

    if(registerIs == RegisterIs::Destination)
    {
        checkReadOnlyRegister(reg.value());
    }


    tryte  first
    {
        trybble{ static_cast<int>(opCode.value())    },
        trybble{ static_cast<int>(reg.value()) }
    };

    tryte  unused{};

    addInstruction(first,unused);
}




void Assembler::parseCode_OpCodeRegisterImmediate      (const SourceLine &source, RegisterIs registerIs)
{
    auto opCode     = source.asOpCode    (0);
    auto reg        = source.asRegister  (1);

    if(!reg)
    {
        error("Invalid register");
    }

    if(registerIs == RegisterIs::Destination)
    {
        checkReadOnlyRegister(reg.value());
    }

    auto operand = parseImmediate (source,2);

    tryte  first
    {
        trybble{ static_cast<int>(opCode.value())    },
        trybble{ static_cast<int>(reg.value())       }
    };


    addInstruction(first,operand);
}



void Assembler::parseCode_OpCodeRegisterRegister   (const SourceLine &source, RegisterIs registerIs)
{
    auto opCode     = source.asOpCode    (0);
    auto reg1       = source.asRegister  (1);
    auto reg2       = source.asRegister  (2);

    if(!reg1)
    {
        error("Invalid first register");
    }

    if(registerIs == RegisterIs::Destination)
    {
        checkReadOnlyRegister(reg1.value());
    }

    if(!reg2)
    {
        error("Invalid second register");
    }

    tryte  first
    {
        trybble{ static_cast<int>(opCode.value())    },
        trybble{ static_cast<int>(reg1.value()) }
    };

    tryte  second
    {
        trybble{ static_cast<int>(reg2.value()) } ,
        trybble{ 0 }
    };

    addInstruction(first,second);
}


void Assembler::parseCode_OpCodeRegisterTrybble        (const SourceLine &source, RegisterIs registerIs)
{
    auto opCode     = source.asOpCode    (0);
    auto reg        = source.asRegister  (1);
    auto t          = source.asTrybble   (2);

    if(!reg)
    {
        error("Invalid register");
    }

    if(registerIs == RegisterIs::Destination)
    {
        checkReadOnlyRegister(reg.value());
    }

    if(!t)
    {
        error("Invalid trybble");
    }

    tryte  first
    {
        trybble{ static_cast<int>(opCode.value())    },
        trybble{ static_cast<int>(reg.value()) }
    };

    tryte  second
    {
        t.value() ,
        trybble{ 0 }
    };

    addInstruction(first,second);
}


void Assembler::parseCode_OpCodeRegisterRegisterTrybble(const SourceLine &source, RegisterIs registerIs)
{
    auto opCode     = source.asOpCode    (0);
    auto reg1       = source.asRegister  (1);
    auto reg2       = source.asRegister  (2);
    auto t          = source.asTrybble   (3);

    if(!reg1)
    {
        error("Invalid first register");
    }

    if(registerIs == RegisterIs::Destination)
    {
        checkReadOnlyRegister(reg1.value());
    }

    if(!reg2)
    {
        error("Invalid second register");
    }

    if(!t)
    {
        error("Invalid trybble");
    }

    tryte  first
    {
        trybble{ static_cast<int>(opCode.value())    },
        trybble{ static_cast<int>(reg1.value()) }
    };

    tryte  second
    {
        trybble{ static_cast<int>(reg2.value()) },
        t.value() ,
    };

    addInstruction(first,second);
}
