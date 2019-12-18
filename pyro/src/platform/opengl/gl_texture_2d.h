#pragma once
#include "pyro/renderer/texture_2d.h" 

namespace pyro
{ 
    class PYRO_API gl_texture_2d : public texture_2d 
    { 
    public: 
        gl_texture_2d(uint32_t width, uint32_t height, texture_parameters params); 
        gl_texture_2d(std::string const &path, texture_parameters params); 
        ~gl_texture_2d(); 
        void bind(uint32_t slot = 0) const override; 
        void data(void *data, uint32_t size) override; 
 
        uint32_t id() const override { return m_id; }
        uint32_t width() const override { return m_width; } 
        uint32_t height() const override { return m_height; } 
        std::string path() const override { return m_path; }

    private: 
        std::string m_path; 
        uint32_t m_id; 
        uint32_t m_width; 
        uint32_t m_height; 
        texture_parameters m_parameters;
        uint32_t m_data_format; 
        uint32_t m_internal_format; 
    };
}
