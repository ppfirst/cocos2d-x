/****************************************************************************
 Copyright (c) 2013 cocos2d-x.org

 http://www.cocos2d-x.org

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/


#ifndef __CCNewTextureAtlas_H_
#define __CCNewTextureAtlas_H_

#include "CCPlatformMacros.h"
#include "CCTextureAtlas.h"

NS_CC_BEGIN

class NewTextureAtlas : public TextureAtlas
{
public:
    static NewTextureAtlas* create(const char* file, long capacity);
    static NewTextureAtlas* createWithTexture(Texture2D *texture, long capacity);

    void drawNumberOfQuads(long numberOfQuads, long start);

protected:
    NewTextureAtlas();
    virtual ~NewTextureAtlas();

};

NS_CC_END

#endif //__CCNewTextureAtlas_H_
