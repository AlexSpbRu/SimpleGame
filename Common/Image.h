#pragma once

bool CreateTextureFromFileGL(LPCTSTR pSrcFile, GLuint& Texture, int& W, int& H, GLenum& Format, GLint MAGfilter, GLint MINfilter, int VerticallyFlip);
bool LoadImageGL(LPCTSTR pSrcFile, int& W, int& H, GLenum& PicFormat, unsigned char** pSrcData, int& SrcDataSize, int VerticallyFlip = 0);
