#include "Core/App.h"

#include <Audio/AudioEngine.h>





int main() {
/*
    App application{};


    application.init();

    application.run();

    application.cleanup();

*/



    // Sound Effect Config
    ma_sound_config soundConfig;

    soundConfig = ma_sound_config_init();
    soundConfig.pFilePath = "../Assets/Audio/bluebonnet_in_b_major_looped.mp3";
    soundConfig.flags = MA_SOUND_FLAG_NO_DEFAULT_ATTACHMENT;
    result = ma_sound_init_ex(&engine, &soundConfig, &g_sound);
    if (result != MA_SUCCESS) {
        return -108;
    }

    // No need for miniaudio to do this, since steam audio does this already
    ma_sound_set_directional_attenuation_factor(&g_sound, 0);

    ma_sound_set_looping(&g_sound, MA_TRUE);


    // steam audio time! :D
    BinauralEffectConfig binauralEffectConfig;

    binauralEffectConfig = initBinauralEffectConfig(CHANNELS, iplAudioSettings, iplContext, iplHRTF);

    result = initBinauralEffect(ma_engine_get_node_graph(&engine), &binauralEffectConfig, NULL, g_binauralEffect);

    if (result != MA_SUCCESS) {
        spdlog::error("Could not initialize binaural Effect: {}", result);
        return -1;
    }

    ma_node_attach_output_bus(&g_binauralEffect, 0, ma_engine_get_endpoint(&engine), 0);

    ma_node_attach_output_bus(&g_sound, 0, &g_binauralEffect, 0);
    result = ma_sound_start(&g_sound);

    if (result != MA_SUCCESS) {
        spdlog::info("Cant start :(");
        return result;
    }

    float stepAngle = 0.002f;
    float angle = 0;
    float distance = 2;


    int i = 0;
    for (;;) {
        double x = ma_cosd(angle) - ma_sind(angle);
        double y = ma_sind(angle) + ma_cosd(angle);
        ma_vec3f direction;

        ma_sound_set_position(&g_sound, (float)x * distance, 0, (float)y * distance);
        direction = ma_sound_get_direction_to_listener(&g_sound);

        g_binauralEffect.direction.x = direction.x;
        g_binauralEffect.direction.y = direction.y;
        g_binauralEffect.direction.z = direction.z;
        angle += stepAngle;

        spdlog::info("D {} {} {}", direction.x, direction.y, direction.z);

        ma_sleep(1);
        if (i > 1000) {
            break;
        }
        i++;
    };



    ma_sound_uninit(&g_sound);
    destroyBinauralEffect(&g_binauralEffect, nullptr);
    ma_engine_uninit(&engine);

    iplContextRelease(&iplContext);
    iplHRTFRelease(&iplHRTF);


    return 0;
}

