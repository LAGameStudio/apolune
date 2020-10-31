#include "OldVBO.h"
#include "Camera.h"
#include "Scene.h"

#include "dhpowareMath.h"

// Macro to return the byte offset into a buffer object in machine units.
// This is used primarily for Buffer Objects.
/*inline const GLubyte *BUFFER_OFFSET(size_t bytes)
{
    return reinterpret_cast<const GLubyte *>(0) + bytes;
}*/

//void VBO16::Render( Vertex *offset, Vertex *position, Vertex *scale, Vertex *rotation, Quaternionf *q, gCamera *c, Scene *s ) {
// VBODebug((GLWindow *)s);
//}