#include <jni.h>
#include <string>
#include <opus.h>
#include <vector>
#include <android/log.h>

#define LOG_TAG "NativeAudio"
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

static OpusDecoder* decoder = nullptr;
static int sample_rate = 48000;
static int channels = 2;

extern "C" JNIEXPORT jstring JNICALL
Java_io_github_mkckr0_audio_1share_1app_MainActivity_stringFromJNI(
        JNIEnv* env,
        jobject /* this */) {
    
    // Check Opus version to confirm linking
    const char* version = opus_get_version_string();
    std::string hello = "Hello from C++ (Opus: " + std::string(version) + ")";
    return env->NewStringUTF(hello.c_str());
}

extern "C" JNIEXPORT jint JNICALL
Java_io_github_mkckr0_audio_1share_1app_service_AudioPlayer_initOpusDecoder(
        JNIEnv* env,
        jobject /* this */,
        jint inSampleRate,
        jint inChannels) {
    
    int error;
    if (decoder != nullptr) {
        opus_decoder_destroy(decoder);
    }
    
    sample_rate = inSampleRate;
    channels = inChannels;
    
    decoder = opus_decoder_create(sample_rate, channels, &error);
    if (error != OPUS_OK) {
        LOGE("Failed to create decoder: %d", error);
        return error;
    }
    return 0;
}

extern "C" JNIEXPORT jint JNICALL
Java_io_github_mkckr0_audio_1share_1app_service_AudioPlayer_decodeOpus(
        JNIEnv* env,
        jobject /* this */,
        jbyteArray encodedData,
        jint len,
        jfloatArray outPcm) {
    
    if (decoder == nullptr) {
        return -1;
    }

    jbyte* encodedBytes = env->GetByteArrayElements(encodedData, nullptr);
    jfloat* pcmBytes = env->GetFloatArrayElements(outPcm, nullptr);
    int maxFrameSize = env->GetArrayLength(outPcm) / channels;

    int decodedSamples = opus_decode_float(decoder, 
                                     (const unsigned char*)encodedBytes, 
                                     len, 
                                     pcmBytes, 
                                     maxFrameSize, 
                                     0);

    if (decodedSamples < 0) {
        LOGE("Decode error: %d", decodedSamples);
    }

    env->ReleaseByteArrayElements(encodedData, encodedBytes, JNI_ABORT);
    env->ReleaseFloatArrayElements(outPcm, pcmBytes, 0);

    return decodedSamples;
}

extern "C" JNIEXPORT void JNICALL
Java_io_github_mkckr0_audio_1share_1app_service_AudioPlayer_destroyOpusDecoder(
        JNIEnv* env,
        jobject /* this */) {
    if (decoder != nullptr) {
        opus_decoder_destroy(decoder);
        decoder = nullptr;
    }
}
