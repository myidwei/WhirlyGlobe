/*
 *  ParticleSystemDrawable.h
 *  WhirlyGlobeLib
 *
 *  Created by Steve Gifford on 4/28/15.
 *  Copyright 2011-2015 mousebird consulting
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 */

#import "Drawable.h"
#import "CoordSystem.h"

namespace WhirlyKit
{
    
// Shader name
#define kParticleSystemShaderName "Default Part Sys (Point)"

// Build the particle system default shader
OpenGLES2Program *BuildParticleSystemProgram();
    
// Low level drawable used to manage particle systems
class ParticleSystemDrawable : public Drawable
{
public:
    // A group of attribute data passed in at once
    class AttributeData
    {
    public:
        std::string name;
        const void *data;
    };
    
    ParticleSystemDrawable(const std::string &name,const std::vector<SingleVertexAttributeInfo> &vertAttrs,int numPoints);
    virtual ~ParticleSystemDrawable();
    
    /// No bounding box, since these change constantly
    Mbr getLocalMbr() const { return Mbr(); }

    /// No offset matrix (at the moment)
    const Eigen::Matrix4d *getMatrix() const { return NULL; }

    /// Draw priority for ordering
    unsigned int getDrawPriority() const { return drawPriority; }
    void setDrawPriority(int newPriority) { drawPriority = newPriority; }

    /// Program to use for rendering
    virtual SimpleIdentity getProgram() const { return programId; }
    /// Set the shader program.  Empty (default) by default
    virtual void setProgram(SimpleIdentity newProgId) { programId = newProgId; }

    /// Whether it's currently displaying
    bool isOn(WhirlyKitRendererFrameInfo *frameInfo) const { return enable; }
    /// True to turn it on, false to turn it off
    void setOnOff(bool onOff) { enable = onOff; }
    
    /// Set the point size
    void setPointSize(float inPointSize) { pointSize = inPointSize; }
    
    /// Set the starting time
    void setStartTime(NSTimeInterval inStartTime) { startTime = inStartTime; }
    NSTimeInterval getStartTime() { return startTime; }
    
    /// Create our buffers in GL
    void setupGL(WhirlyKitGLSetupInfo *setupInfo,OpenGLMemManager *memManager);
    
    /// Destroy GL buffers
    void teardownGL(OpenGLMemManager *memManager);
    
    /// Called on the rendering thread to draw
    void draw(WhirlyKitRendererFrameInfo *frameInfo,Scene *scene);
    
    /// Just points for now
    GLenum getType() const { return GL_POINTS; }
    
    /// Not using this mechanism
    bool hasAlpha(WhirlyKitRendererFrameInfo *frameInfo) const { return false; }
    
    /// Don't need to update the renderer particularly
    void updateRenderer(WhirlyKitSceneRendererES *renderer);

    /// If set, we want to use the z buffer
    bool getRequestZBuffer() const { return requestZBuffer; }
    void setRequestZBuffer(bool enable) { requestZBuffer = enable; }
    
    /// If set, we want to write to the z buffer
    bool getWriteZbuffer() const { return writeZBuffer; }
    void setWriteZbuffer(bool enable) { writeZBuffer = enable; }
    
    /// Number of points we can expect in the attribute data
    void setNumPoints(int inPts) { numPoints = inPts; }
    
    /// Add the vertex data (all of it) at once
    void addAttributeData(const std::vector<AttributeData> &attrData);

protected:
    void setupVAO(OpenGLES2Program *prog);
    
    bool enable;
    int numPoints;
    int vertexSize;
    std::vector<SingleVertexAttributeInfo> vertAttrs;
    SimpleIdentity programId;
    int drawPriority;
    float pointSize;
    NSTimeInterval startTime;
    bool requestZBuffer,writeZBuffer;
    float minVis,maxVis,minVisibleFadeBand,maxVisibleFadeBand;
    GLuint pointBuffer;
    GLuint vertArrayObj;

    // The vertex attributes we're representing in the buffers
    std::vector<VertexAttribute> vertexAttributes;
};
    
}