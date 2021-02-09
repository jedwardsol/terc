#pragma once

#include <filesystem>
namespace fs=std::filesystem;

#include <fstream>
#include <vector>
#include <string>
using namespace std::literals;
#include <exception>
#include <system_error>
#include <cerrno>
#include "Arithmetic/tryte.h"



class MemoryBlock 
{
public:

    MemoryBlock(int size,  std::string  filename) : memory(size,tryte{0}) , filename{std::move(filename)}
    {}

    ~MemoryBlock()
    {
    
        std::ofstream   file{filename, std::ios::binary | std::ios::out};

        if(file)
        {
            file.write(reinterpret_cast<char*>(memory.data()), memory.size() * sizeof(tryte));                
        }    
    }

    tryte &operator[](int index)
    {
        return memory.at(index);
    }


protected:

    std::vector<tryte>      memory;
    std::string             filename;
};




class ROMemoryBlock
{
public:

    ROMemoryBlock(int size) : memory(size,tryte{0})
    {
    }

    ROMemoryBlock(int size, const std::string  &filename) : memory(size,tryte{0})
    {
        auto fileSize = fs::file_size(filename);

        if(fileSize != size*sizeof(tryte))
        {
            throw std::runtime_error{filename + " is the wrong size"s};
        }

        std::ifstream   file{filename, std::ios::binary | std::ios::in};

        if(!file)
        {
            throw std::system_error{errno,std::generic_category(),"Opening "s + filename};
        }

        file.read(reinterpret_cast<char*>(memory.data()), memory.size() * sizeof(tryte));                

        if(!file)
        {
            throw std::system_error{errno,std::generic_category(),"Reading "s + filename};
        }

        if(file.gcount() != fileSize)
        {
            throw std::runtime_error{"read the wrong number of bytes from "s + filename };
        }
    }

    const tryte &operator[](int index)
    {
        return memory.at(index);
    }


protected:

    std::vector<tryte>      memory;
};


class RWMemoryBlock : public ROMemoryBlock
{
public:

    RWMemoryBlock(int size) : ROMemoryBlock{size}
    {}

    RWMemoryBlock(int size,  std::string  filename) : ROMemoryBlock{size,filename} , filename{std::move(filename)}
    {}

    ~RWMemoryBlock()
    {
        if(!filename.empty())    
        {
            std::ofstream   file{filename + ".final", std::ios::binary | std::ios::out};

            if(file)
            {
                file.write(reinterpret_cast<char*>(memory.data()), memory.size() * sizeof(tryte));                
            }    
        }
    }

    tryte &operator[](int index)
    {
        return memory.at(index);
    }


protected:

    std::string             filename;
};
