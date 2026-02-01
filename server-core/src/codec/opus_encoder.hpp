#ifndef OPUS_ENCODER_HPP
#define OPUS_ENCODER_HPP

#include <opus/opus.h>
#include <vector>
#include <memory>
#include <stdexcept>

class audio_opus_encoder {
public:
    audio_opus_encoder(int sample_rate, int channels, int bitrate = 128000)
        : _channels(channels) {
        int error;
        _encoder = opus_encoder_create(sample_rate, channels, OPUS_APPLICATION_AUDIO, &error);
        if (error != OPUS_OK) {
            throw std::runtime_error("Failed to create Opus encoder");
        }
        
        opus_encoder_ctl(_encoder, OPUS_SET_BITRATE(bitrate));
        opus_encoder_ctl(_encoder, OPUS_SET_COMPLEXITY(10));
        opus_encoder_ctl(_encoder, OPUS_SET_SIGNAL(OPUS_SIGNAL_MUSIC));
    }

    ~audio_opus_encoder() {
        if (_encoder) {
            opus_encoder_destroy(_encoder);
        }
    }

    // Encodes interleaved PCM samples (int16_t) to Opus
    int encode(const int16_t* pcm, int frame_size, unsigned char* compressed_data, int max_bytes) {
        return opus_encode(_encoder, pcm, frame_size, compressed_data, max_bytes);
    }

    // Encodes interleaved PCM samples (float) to Opus
    int encode(const float* pcm, int frame_size, unsigned char* compressed_data, int max_bytes) {
        return opus_encode_float(_encoder, pcm, frame_size, compressed_data, max_bytes);
    }

private:
    OpusEncoder* _encoder;
    int _channels;
};

#endif // OPUS_ENCODER_HPP
