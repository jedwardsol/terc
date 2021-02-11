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
#include <cstdint>
#include "Arithmetic/tryte.h"



namespace Architecture
{


class MemoryBlock 
{
public:

    int negativeSize() const noexcept 
    {
        return -zeroOffset;
    }

    int positiveSize() const noexcept 
    {                  
        return totalSize - zeroOffset;
    }

#pragma warning(push)
#pragma warning(disable: 26451)    // overflow in indexing operation

    tryte &operator[](int index)
    {
        return memory.at(index+zeroOffset);
    }

    const tryte &operator[](int index) const
    {
        return memory.at(index+zeroOffset);
    }

#pragma warning(pop)

public:

// sized

    MemoryBlock(int32_t         negativeSize,
                int32_t         positiveSize,
                std::string     saveFilename) : zeroOffset  {negativeSize},
                                                totalSize   {negativeSize+positiveSize},
                                                memory      (totalSize, tryte{}),
                                                saveFilename{std::move(saveFilename)}
    {}


    MemoryBlock(int32_t         negativeSize,
                int32_t         positiveSize) : MemoryBlock{negativeSize,positiveSize,{}}
    {}


// file based

    MemoryBlock(const std::string       &loadFilename,
                      std::string        saveFilename) : saveFilename{std::move(saveFilename)}
    {
// check and open

        auto fileSize = fs::file_size(loadFilename);

        if(fileSize < 2 * sizeof(int32_t))
        {
            throw std::runtime_error{loadFilename + " is much too small"s};
        }

        std::ifstream   file{loadFilename, std::ios::binary | std::ios::in};

        if(!file)
        {
            throw std::system_error{errno,std::generic_category(),"Opening "s + loadFilename};
        }

// read sizes and check

        file.read(reinterpret_cast<char*>(&zeroOffset), sizeof(int32_t));                
        file.read(reinterpret_cast<char*>(&totalSize),  sizeof(int32_t));                

        if(!file)
        {
            throw std::system_error{errno,std::generic_category(),"Reading sizes from  "s + loadFilename};
        }


        if(fileSize !=  (2*sizeof(int32_t))  + (totalSize*sizeof(tryte)))
        {
            throw std::runtime_error{loadFilename + " is the wrong size"s};
        }

// read data

        memory.resize(totalSize);

        file.read(reinterpret_cast<char*>(memory.data()), memory.size() * sizeof(tryte));                

        if(!file)
        {
            throw std::system_error{errno,std::generic_category(),"Reading "s + loadFilename};
        }

        if(file.gcount() != fileSize)
        {
            throw std::runtime_error{"read the wrong number of bytes from "s + loadFilename};
        }
    }

    MemoryBlock(const std::string       &loadFilename) : MemoryBlock{ loadFilename, {}}
    {}


    ~MemoryBlock()
    {
        if(!saveFilename.empty())
        {
            std::ofstream   file{saveFilename, std::ios::binary | std::ios::out};

            if(file)
            {
                file.write(reinterpret_cast<char*>(&zeroOffset), sizeof(int32_t));                
                file.write(reinterpret_cast<char*>(&totalSize),  sizeof(int32_t));                

                file.write(reinterpret_cast<char*>(memory.data()), memory.size() * sizeof(tryte));                
            }    
        }
    }


protected:

    int32_t                 zeroOffset;
    int32_t                 totalSize;
    std::vector<tryte>      memory;

    std::string             saveFilename;
};


}


