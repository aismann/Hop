#ifndef COLLISIONMESHDEBUG_H
#define COLLISIONMESHDEBUG_H

#include <Collision/collisionMesh.h>
#include <Object/entityComponentSystem.h>
#include <Component/cRenderable.h>

#include <jGL/jGL.h>

namespace Hop::Object
{
    class EntityComponentSystem;
}

namespace Hop::Debugging
{
    using Hop::Object::EntityComponentSystem;
    using Hop::Object::Component::cCollideable;
    using Hop::System::Physics::CollisionPrimitive;
    using Hop::System::Physics::MeshPoint;
    using Hop::System::Physics::Rectangle;
    using Hop::Object::Component::cRenderable;

    class CollisionMeshDebug
    {

    public:

        CollisionMeshDebug(std::shared_ptr<jGL::jGLInstance> jgl)
        : refresh(true),
          circleShader(collisionPrimitiveVertexShader, collisionPrimitiveFragmentShader),
          rectangleShader(rectangleVertexShader, rectangleFragmentShader)
        {
            shapes = jgl->createShapeRenderer(256);
        }

        ~CollisionMeshDebug()
        {
        }

        void drawMeshes(EntityComponentSystem * m, glm::mat4 proj);

        void refreshMeshes() { refresh = true; }

    private:

        bool refresh;

        std::shared_ptr<jGL::ShapeRenderer> shapes;

        jGL::GL::glShader circleShader, rectangleShader;

        const char * collisionPrimitiveVertexShader = "#version " GLSL_VERSION "\n"
        "precision lowp float;\n precision lowp int;\n"
        "layout(location=0) in vec4 a_position;\n"
        "layout(location=1) in vec4 a_offset;\n"
        "layout(location=2) in vec4 a_colour;\n"
        "uniform mat4 proj;\n"
        "out vec2 texCoord;\n"
        "out vec4 oColour;\n"
        "void main()\n"
        "{\n"
            "gl_Position = proj*vec4(a_offset.w*a_position.xy+a_offset.xy,0.0,1.0);\n"
            "texCoord = a_position.zw;\n"
            "oColour = a_colour;\n"
        "}";

        const char * collisionPrimitiveFragmentShader = "#version " GLSL_VERSION "\n"
        "precision lowp float;\n precision lowp int;\n"
        "in vec4 oColour;\n"
        "in vec2 texCoord;\n"
        "out vec4 colour;\n"
        "float t0 = 0.12;\n"
        "float t1 = 0.2025;\n"
        "float t2 = 0.25;\n"
        "void main()"
        "{\n"
        "   vec2 c = texCoord-vec2(0.5,0.5);\n"
        "   float d2 = dot(c,c);"
        "   if (d2 > t2) {discard;}\n"
        "   float alpha = 1.0-smoothstep(t1, t2, d2);"
        "   float mixer = 1.0-smoothstep(t0, t1, d2);"
        "   colour = vec4( mix(vec3(0.0,0.0,0.0),oColour.rgb, mixer), alpha );"
        "}";

        const char * rectangleVertexShader = "#version " GLSL_VERSION "\n"
        "precision lowp float;\n precision lowp int;\n"
        "layout(location=0) in vec4 a_position;\n"
        "layout(location=1) in vec4 a_offset;\n"
        "layout(location=2) in vec4 a_parameters;\n"
        "layout(location=3) in float a_thickness;\n"
        "uniform mat4 proj;\n"
        "out vec4 oColour;\n"
        "out vec2 texCoord;\n"
        "out vec4 parameters;\n"
        "out float th;\n"
        "void main()\n"
        "{\n"
            "float cosine = cos(a_offset.z); float sine = sin(a_offset.z);\n"
            "mat2 rot = mat2(cosine,-sine,sine,cosine);"
            "vec2 pos = rot * a_position.xy;\n"
            "gl_Position = proj*vec4(a_offset.w*pos+a_offset.xy,0.0,1.0);\n"
            "texCoord = pos;\n"
            "parameters.xy = rot * a_parameters.xy;\n"
            "parameters.zw = rot * a_parameters.zw;\n"
            "oColour = vec4(200.0/255.0,200.0/255.0,250.0/255.0,1.0);\n"
            "th = a_thickness;\n"
        "}";

        const char * rectangleFragmentShader = "#version " GLSL_VERSION "\n"
        "precision lowp float;\n precision lowp int;\n"
        "in vec4 oColour;\n"
        "in vec2 texCoord;\n"
        "in float th;\n"
        "in vec4 parameters;\n"
        "out vec4 colour;\n"
        "void main()"
        "{\n"
            "vec2 a = parameters.xy; vec2 b = parameters.zw;"
            "float l = length(b-a);"
            "vec2 d = (b-a)/l;\n"
            "vec2 q = texCoord - (a+b)*0.5;\n"
            "q = mat2(d.x,-d.y,d.y,d.x)*q;\n"
            "q = abs(q)-vec2(l*0.5,th);\n"
            "float sd = length(max(q,0.0)) + min(max(q.x,q.y),0.0);"
            "float alpha = 1.0;"
            "if (sd <= 0.0) { colour = vec4(oColour.rgb,0.5); }"
            //"if (sd <= 0.1){ alpha = 1.0-smoothstep(0.0,0.1,sd); colour = vec4(oColour.rgb,alpha); }"
            "else { colour = vec4(oColour.rgb,alpha); }"
        "}";

    };
}

#endif /* COLLISIONMESHDEBUG_H */
