#pragma once
#include "pyro/logger.h"
#include "pyro/renderer/texture.h" 

namespace pyro 
{ 
    namespace utils 
    { 
        static std::string type_to_string(e_type format) 
        { 
            switch(format) 
            { 
                case e_type::jpeg: return "jpg"; 
                case e_type::png:  return "png";
                default: break;
            } 

            PYRO_CORE_ASSERT(false, "[texture] Unknown renderer api!"); 
            return "Unknown"; 
        } 

        static e_type format_from_extension(const std::string& ext) 
        { 
            if(ext == "jpg") 
                return e_type::jpeg; 
            if(ext == "png") 
                return e_type::png; 

            PYRO_CORE_ASSERT(false, "[texture] Unknown renderer api!"); 
            return e_type::unknown; 
        } 

    }    
} 
