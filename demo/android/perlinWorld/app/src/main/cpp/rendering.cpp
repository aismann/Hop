extern "C"
{
    void Java_app_jerboa_glskeleton_Hop_render(
            JNIEnv *env,
            jobject /* this */,
            jboolean refresh
    )
    {
        if (manager == nullptr){
            return;
        }

        sRender & rendering = manager->getSystem<sRender>();

        rendering.setProjection(camera->getVP());

//        world->draw(*shaderPool->get("worldShader").get());
//
//        rendering.update(manager, shaderPool, refresh);
        if (refresh){ rendering.refreshShaders(); }
        rendering.draw(manager, world);

//        rendering.draw(shaderPool);
        //hop->log<Hop::Logging::INFO>("refresh = "+std::to_string(refresh));
    }

    void Java_app_jerboa_glskeleton_Hop_renderText
            (
                    JNIEnv *env,
                    jobject /* this */,
                    jstring text,
                    jfloat x,
                    jfloat y,
                    jfloat scale,
                    jfloat r,
                    jfloat g,
                    jfloat b,
                    jfloat a,
                    jboolean centred
            )
    {
        if (textRenderer == nullptr){
            return;
        }

        textRenderer->renderText
                (
                        *font,
                        jstring2string(env,text).c_str(),
                        x,
                        y,
                        scale,
                        glm::vec3(r,g,b),
                        a,
                        centred
                );
    }
}