#ifndef FRAME_BUFFER_HPP
#define FRAME_BUFFER_HPP

#include "texture.hpp"

class frame_buffer
{
public:
    frame_buffer();
    ~frame_buffer();

    void bind() const;
    void bind_texture(const texture& texture) const;

private:
    unsigned int id;

};

#endif // FRAME_BUFFER_HPP
